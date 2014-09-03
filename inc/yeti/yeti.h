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

#ifndef INC_YETI_YETI_H_
#define INC_YETI_YETI_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include <yeti/color.h>

namespace yeti {

/** @brief Constants to set current logging level. */
enum LogLevel {
  LOG_LEVEL_CRITICAL,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_TRACE
};

/** @brief Sets logging level. */
void SetLevel(LogLevel level) noexcept;

/** @brief Returns current logging level. */
int GetLevel() noexcept;

/** @brief Sets log colorization: "true" to set colored log. */
void SetColored(bool is_colored) noexcept;

/** @brief Returns current log colorization. */
bool GetColored() noexcept;

/**
 * @brief Sets file descriptor to log into it.
 *
 * Logging is in separate thread, so you don't know when it is possible to close
 * log file. To overcome this you should use yeti::CloseFIle(fd).
 * This function adds to execution queue ::fclose(fd).
 */
void SetFileDesc(FILE* fd) noexcept;

/** @brief Returns current log file descriptor. */
FILE* GetFileDesc() noexcept;

/**
 * @brief Closes specified log file descriptor.
 *
 * If fd is equal to nullptr, current log file descriptor will be closed.
 */
void CloseFile(FILE* fd = nullptr);

/**
 * @brief Sets current log file format.
 *
 * Keywords to set log format are:
 * <ul>
 * <li> %(TAG)      - tag of current logging level </li>
 * <li> %(FILENAME) - filename </li>
 * <li> %(FUNCNAME) - function name </li>
 * <li> %(PID)      - process ID </li>
 * <li> %(TID)      - thread ID </li>
 * <li> %(LINE)     - line number </li>
 * <li> %(MSG)      - user message (format string) </li>
 * </ul>
 *
 * You should always use %(MSG) in format string.
 */
void SetFormatStr(const std::string& format_str) noexcept;

/** @brief Returns current format string. */
std::string GetFormatStr() noexcept;


// user should not see _Shutdown() in yeti namespace
namespace { void _Shutdown(); }

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
  void PushToLog(const std::function<void()>& queue_func);

  /** @brief Sets logging level. */
  void SetLevel(LogLevel level) noexcept { instance().level_ = level; }
  /** @brief Returns current logging level. */
  int GetLevel() const noexcept { return instance().level_; }

  /** @brief Sets log colorization. */
  void SetColored(bool is_colored) noexcept { is_colored_ = is_colored; }
  /** @brief Returns current log colorization. */
  bool GetColored() const noexcept { return instance().is_colored_; }

  /** @brief Sets file log descriptor. */
  void SetFileDesc(FILE* fd) noexcept { fd_ = fd; }
  /** @brief Returns current file log descriptor. */
  FILE* GetFileDesc() const noexcept { return fd_;}
  /** @brief Closes specified log file descriptor. */
  void CloseFile(FILE* fd = nullptr);

  /** @brief Sets specified log format. */
  void SetFormatStr(const std::string& format_str) noexcept;
  /** @brief Returns current log format. */
  std::string GetFormatStr() const noexcept;

  /** @brief Contains loop of logging thread. */
  void Print();

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
  std::thread thread_;
  std::atomic<FILE*> fd_;
};

}  // namespace yeti

#include <yeti/yeti-inl.h>
#include <yeti/macro.h>

#endif  // INC_YETI_YETI_H_
