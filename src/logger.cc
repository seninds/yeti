// Copyright (c) 2014, Dmitry Senin (seninds@gmail.com)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   1. Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// yeti - C++ lightweight threadsafe logging
// URL: https://github.com/seninds/yeti.git

#include <cstdlib>
#include <list>
#include <algorithm>
#include <map>

#include <src/logger.h>

namespace yeti {

Logger::Logger()
    : stop_loop_(false),
      is_colored_(true),
      level_(LogLevel::LOG_LEVEL_INFO),
      msg_id_(0),
      format_str_("[%(LEVEL)] %(FILENAME): %(LINE): %(MSG)"),
      fd_(stderr) {
  thread_ = std::thread(&Logger::ProcessingLoop, this);

  // check environment variable to set log level
  level_ = Logger::LogLevelFromEnv(std::getenv("YETI_LOG_LEVEL"));
}

LogLevel Logger::LogLevelFromEnv(const char* var) {
  if (var == nullptr) return LogLevel::LOG_LEVEL_INFO;

  std::map<std::vector<std::string>, LogLevel> level_dict = {
    {{"TRACE", "TRC", "trace", "trc"}, LogLevel::LOG_LEVEL_TRACE},
    {{"DEBUG", "DBG", "debug", "dbg"}, LogLevel::LOG_LEVEL_DEBUG},
    {{"INF", "inf"}, LogLevel::LOG_LEVEL_INFO},
    {{"WARN", "WRN", "warn", "wrn"}, LogLevel::LOG_LEVEL_WARNING},
    {{"ERR", "err"}, LogLevel::LOG_LEVEL_ERROR},
    {{"CRIT", "CRT", "crit", "crt"}, LogLevel::LOG_LEVEL_CRITICAL}
  };

  const std::string env_str = var;
  auto it = std::find_if(level_dict.begin(), level_dict.end(),
                         [&env_str] (const std::pair<std::vector<std::string>, LogLevel>& p) {
                           if (std::find_if(p.first.begin(), p.first.end(),
                                            [&env_str](const std::string& s) {
                                              if (env_str.find(s) != std::string::npos) {
                                                return true;
                                              }
                                              return false;
                                            })
                               != p.first.end()) {
                             return true;
                           }
                           return false;
                         });
  if (it != level_dict.end()) {
    return it->second;
  }
  return LogLevel::LOG_LEVEL_INFO;
}


void RegisterSignals();

Logger& Logger::instance() {
  static Logger logger;

  // register signals to flush log on them
  // (logger instance should have been already created at this moment)
  static bool is_registered = false;
  static std::mutex mutex;
  if (!is_registered) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!is_registered) {
      RegisterSignals();
      is_registered = true;
    }
  }

  return logger;
}

void Logger::CloseFileDesc(FILE* fd) {
  if (fd == nullptr) {
    fd = fd_;
  }
  if (fd != stderr && fd != stdout && fd != stdin) {
    auto close_func = [fd] { std::fclose(fd); };
    this->EnqueueTask(close_func);
  }
}

void Logger::EnqueueTask(const std::function<void()>& queue_func) {
  std::lock_guard<std::mutex> queue_lock(queue_mutex_);
  queue_.push(queue_func);
  cv_.notify_one();
}

void Logger::Shutdown() {
  // set flag to stop processing loop
  stop_loop_ = true;
  cv_.notify_one();
  if (thread_.joinable()) {
    thread_.join();
  }
}

void Logger::ProcessingLoop() {
  // start processing loop
  do {
    std::unique_lock<std::mutex> queue_lock(queue_mutex_);
    cv_.wait(queue_lock, [this] { return !this->queue_.empty(); });

    // build execution list
    std::list<std::function<void()>> exec_list;
    while (!queue_.empty()) {
      exec_list.push_back(queue_.front());
      queue_.pop();
    }
    queue_mutex_.unlock();

    // execute all elements from execution list
    for (const auto& functor : exec_list) {
      functor();
    }
  } while (!stop_loop_ || queue_.size());
}

void Logger::SetFormatStr(const std::string& format_str) noexcept {
  std::lock_guard<std::mutex> lock(settings_mutex_);
  format_str_ = format_str;
}

std::string Logger::GetFormatStr() const noexcept {
  std::lock_guard<std::mutex> lock(settings_mutex_);
  return format_str_;
}

}  // namespace yeti
