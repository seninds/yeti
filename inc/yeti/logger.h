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

#ifndef INC_YETI_LOGGER_H_
#define INC_YETI_LOGGER_H_

#include <cstdio>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <yeti/log_level.h>

namespace yeti {

/** @brief Singleton to provide access to logger object. */
class Logger {
 public:
  ~Logger() = default;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger* operator &() = delete;

  /** @brief Returns instance of logger object. */
  static Logger& instance();

  /** @brief Adds functor to log queue. */
  void EnqueueTask(const std::function<void()>& queue_func);

  /** @brief Sets logging level. */
  void SetLevel(LogLevel level) noexcept { instance().level_ = level; }
  /** @brief Returns current logging level. */
  int GetLevel() const noexcept { return instance().level_; }

  /** @brief Sets log colorization. */
  void SetColored(bool is_colored) noexcept { is_colored_ = is_colored; }
  /** @brief Returns current log colorization. */
  bool IsColored() const noexcept { return instance().is_colored_; }

  /** @brief Sets file log descriptor. */
  void SetFileDesc(FILE* fd) noexcept { fd_ = fd; }
  /** @brief Returns current file log descriptor. */
  FILE* GetFileDesc() const noexcept { return fd_;}
  /** @brief Closes specified log file descriptor. */
  void CloseFileDesc(FILE* fd = nullptr);

  /** @brief Sets specified log format. */
  void SetFormatStr(const std::string& format_str) noexcept;
  /** @brief Returns current log format. */
  std::string GetFormatStr() const noexcept;

  /** @brief Contains loop of logging thread. */
  void ProcessingLoop();

  /** @brief Shutdowns logging. */
  void Shutdown();

 private:
  Logger();

  mutable std::mutex queue_mutex_;
  mutable std::mutex settings_mutex_;
  std::condition_variable cv_;
  std::queue<std::function<void()>> queue_;
  std::atomic<bool> stop_loop_;
  std::atomic<bool> is_colored_;
  std::atomic<int> level_;
  std::string format_str_;
  std::atomic<FILE*> fd_;
  std::thread thread_;
};

}  // namespace yeti

#endif  // INC_YETI_LOGGER_H_
