/**
 * @file yeti.h
 * @brief Base functions to control logging process.
 */

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

#ifndef INC_YETI_YETI_H_
#define INC_YETI_YETI_H_

#include <cstdio>
#include <cstdlib>
#include <string>

/**
 * @namespace yeti
 * <b>yeti</b> namespace contains all necessary functions to control logging.
 */
namespace yeti {

/**
 * Macros for logging (printf-like format):
 *   TRACE(msg_fmt, ...);
 *   DEBUG(msg_fmt, ...);
 *   INFO(msg_fmt, ...);
 *   WARN(msg_fmt, ...);
 *   ERROR(msg_fmt, ...);
 *   CRITICAL(msg_fmt, ...);
 */

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
void SetLogLevel(LogLevel level) noexcept;

/** @brief Returns current logging level. */
int GetLogLevel() noexcept;

/** @brief Sets log to be colored. */
void SetLogColored(bool is_colored) noexcept;

/** @brief Returns is log colored. */
bool IsLogColored() noexcept;

/**
 * @brief Sets file descriptor to log into it.
 *
 * Logging is in separate thread, so you don't know when it is possible to close
 * log file. To overcome this you should use yeti::CloseFIle(fd).
 * This function adds to execution queue ::fclose(fd).
 */
void SetLogFileDesc(FILE* fd) noexcept;

/** @brief Returns current log file descriptor. */
FILE* GetLogFileDesc() noexcept;

/**
 * @brief Closes specified log file descriptor.
 *
 * If fd is equal to nullptr, current log file descriptor will be closed.
 */
void CloseLogFileDesc(FILE* fd = nullptr);

/**
 * @brief Sets current log file format.
 *
 * Keywords to set log format are:
 * <ul>
 * <li> %(LEVEL)    - current logging level </li>
 * <li> %(FILENAME) - filename </li>
 * <li> %(FUNCNAME) - function name </li>
 * <li> %(PID)      - process ID </li>
 * <li> %(TID)      - thread ID </li>
 * <li> %(LINE)     - line number </li>
 * <li> %(MSG)      - user message (format string) </li>
 * <li> %(MSG_ID)   - unique message number to refer in discussion with colleagues </li>
 * <li> %(DATE)     - local date in YYYY-MM-DD format (the ISO 8601 date format) </li>
 * <li> %(TIME)     - local time in HH:MM:SS.SSS format (based on the ISO 8601 time format) </li>
 * </ul>
 *
 * You should always use %(MSG) in format string if you want to log user message.
 */
void SetLogFormatStr(const std::string& format_str) noexcept;

/** @brief Returns current format string. */
std::string GetLogFormatStr() noexcept;

/** @brief Flush log queue (blocking call). */
void FlushLog();

}  // namespace yeti

#include <yeti/macro.h>

#endif  // INC_YETI_YETI_H_
