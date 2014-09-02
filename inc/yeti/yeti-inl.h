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
// yeti - C++ lightweight threadsafe logging system
// URL: https://github.com/seninds/yeti.git

#ifndef INC_YETI_YETI_INL_H_
#define INC_YETI_YETI_INL_H_

namespace yeti {

namespace {
  inline void _Shutdown() { yeti::Logger::instance().Shutdown(); }
}

inline void SetLevel(LogLevel level) noexcept {
  Logger::instance().SetLevel(level);
}

inline int GetLevel() noexcept {
  return Logger::instance().GetLevel();
}

inline void SetColored(bool is_colored) noexcept {
  Logger::instance().SetColored(is_colored);
}

inline bool GetColored() noexcept {
  return Logger::instance().GetColored();
}

inline void SetFileDesc(FILE* fd) noexcept {
  Logger::instance().SetFileDesc(fd);
}

inline FILE* GetFileDesc() noexcept {
  return Logger::instance().GetFileDesc();
}

inline void CloseFile(FILE* fd = nullptr) {
  Logger::instance().CloseFile(fd);
}


inline Logger::Logger()
    : stop_loop_(false),
      is_colored_(true),
      level_(LogLevel::LOG_LEVEL_INFO),
      fd_(stderr) {
  thread_ = std::thread(&Logger::Print, this);
}

inline Logger& Logger::instance() {
  static Logger logger;

  // register _Shutdown() at std::exit()
  static bool is_atexit_registered = false;
  static std::mutex registered_mutex;
  if (!is_atexit_registered) {
    std::lock_guard<std::mutex> lock(registered_mutex);
    if (!is_atexit_registered) {
      std::atexit(_Shutdown);
      is_atexit_registered = true;
    }
  }
  return logger;
}

inline void Logger::CloseFile(FILE* fd) {
  if (fd == nullptr) {
    fd = fd_;
  }
  auto close_func = [fd] { ::fclose(fd); };
  this->PushToQueue(close_func);
}

inline void Logger::PushToQueue(const std::function<void()>& queue_func) {
  std::lock_guard<std::mutex> queue_lock(queue_mutex_);
  queue_.push(queue_func);
  cv_.notify_one();
}

inline void Logger::Shutdown() {
  // set flag to stop processing loop
  stop_loop_ = true;
  if (thread_.joinable()) {
    thread_.join();
  }
}

inline void Logger::Print() {
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

}  // namespace yeti

#endif  // INC_YETI_YETI_INL_H_
