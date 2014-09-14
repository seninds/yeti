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

#include <yeti/logger.h>

namespace yeti {

Logger::Logger()
    : stop_loop_(false),
      is_colored_(true),
      level_(LogLevel::LOG_LEVEL_INFO),
      msg_id_(0),
      format_str_("[%(LEVEL)] %(FILENAME): %(LINE): %(MSG)"),
      fd_(stderr) {
  thread_ = std::thread(&Logger::ProcessingLoop, this);
}

void RegisterSignals();

Logger& Logger::instance() {
  static Logger logger;

  // register signals to flush log on them
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
  if (thread_.joinable()) {
    thread_.join();
  }
}

void Logger::ProcessingLoop() {
  // start processing loop
  do {
    std::unique_lock<std::mutex> queue_lock(queue_mutex_);
    cv_.wait_for(queue_lock, std::chrono::milliseconds(1000),
                 [this] { return !this->queue_.empty(); });
    while (!queue_.empty()) {
      queue_.front()();
      queue_.pop();
    }
  } while (!stop_loop_);
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
