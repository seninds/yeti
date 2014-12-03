/**
 * @file macro.h
 * @brief Macro definitions for logging.
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

#ifndef INC_YETI_MACRO_H_
#define INC_YETI_MACRO_H_

#include <unistd.h>
#include <cstdio>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <yeti/yeti.h>

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wformat-security"
#elif defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic ignored "-Wformat-security"
#endif  // ignored "-Wformat-security"


#define MAX_MSG_LENGTH 512

/// @cond

#define YETI_BALCK   "\033[0;30m"
#define YETI_ORANGE  "\033[0;33m"
#define YETI_BLUE    "\033[0;34m"
#define YETI_GREEN   "\033[0;32m"
#define YETI_CYAN    "\033[0;36m"
#define YETI_RED     "\033[0;31m"
#define YETI_PURPLE  "\033[0;35m"
#define YETI_BROWN   "\033[0;33m"
#define YETI_GRAY    "\033[0;37m"

#define YETI_DGRAY   "\033[1;30m"
#define YETI_LBLUE   "\033[1;34m"
#define YETI_LGREEN  "\033[1;32m"
#define YETI_LCYAN   "\033[1;36m"
#define YETI_LRED    "\033[1;31m"
#define YETI_LPURPLE "\033[1;35m"
#define YETI_YELLOW  "\033[1;33m"
#define YETI_WHITE   "\033[1;37m"

#define YETI_HIGH    "\033[1m"
#define YETI_RESET   "\033[0m"

namespace yeti {

struct LogData {
  std::string log_format;
  std::string level;
  std::string color;
  std::string filename;
  std::string funcname;
  pid_t pid;
  std::thread::id tid;
  int line;
  FILE* fd;
  std::size_t msg_id;
  std::string msg;
  std::chrono::high_resolution_clock::time_point time;
};

// ------------ auxiliary functions ------------
void _EnqueueLogTask(yeti::LogData* log_data);
std::size_t _GetMsgId();
void _IncMsgId();
// ---------------------------------------------

}  // namespace yeti

// @endcond

#ifdef DISABLE_LOGGING

#define CRITICAL(fmt, ...) ((void) 0)
#define ERROR(fmt, ...) ((void) 0)
#define WARN(fmt, ...) ((void) 0)
#define INFO(fmt, ...) ((void) 0)
#define DEBUG(fmt, ...) ((void) 0)
#define TRACE(fmt, ...) ((void) 0)

#else  // DISABLE_LOGGING

/**
 * @brief Logs critical error message using specified printf-like format.
 */
#define CRITICAL(fmt, ...) { \
  yeti::LogData yeti_log_data; \
  yeti_log_data.level = "CRITICAL"; \
  yeti_log_data.color = YETI_LRED; \
  yeti_log_data.filename = __FILE__; \
  yeti_log_data.funcname = __func__; \
  yeti_log_data.line = __LINE__; \
  yeti_log_data.msg_id = yeti::_GetMsgId(); \
  yeti::_IncMsgId(); \
  \
  char yeti_log_msg[MAX_MSG_LENGTH] = { 0 }; \
  std::snprintf(yeti_log_msg, sizeof(yeti_log_msg), fmt, ##__VA_ARGS__); \
  yeti_log_data.msg = yeti_log_msg; \
  \
  yeti::_EnqueueLogTask(&yeti_log_data); \
}

/**
 * @brief Logs error message using specified printf-like format.
 */
#define ERROR(fmt, ...) { \
  std::size_t yeti_lot_msg_id = yeti::_GetMsgId(); \
  yeti::_IncMsgId(); \
  if (yeti::GetLogLevel() >= yeti::LOG_LEVEL_ERROR) { \
    yeti::LogData yeti_log_data; \
    yeti_log_data.level = "ERROR"; \
    yeti_log_data.color = YETI_LPURPLE; \
    yeti_log_data.filename = __FILE__; \
    yeti_log_data.funcname = __func__; \
    yeti_log_data.line = __LINE__; \
    yeti_log_data.msg_id = yeti_lot_msg_id; \
    \
    char yeti_log_msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(yeti_log_msg, sizeof(yeti_log_msg), fmt, ##__VA_ARGS__); \
    yeti_log_data.msg = yeti_log_msg; \
    \
    yeti::_EnqueueLogTask(&yeti_log_data); \
  } \
}

/**
 * @brief Logs warning message using specified printf-like format.
 */
#define WARN(fmt, ...) { \
  std::size_t yeti_lot_msg_id = yeti::_GetMsgId(); \
  yeti::_IncMsgId(); \
  if (yeti::GetLogLevel() >= yeti::LOG_LEVEL_WARNING) { \
    yeti::LogData yeti_log_data; \
    yeti_log_data.level = "WARNING"; \
    yeti_log_data.color = YETI_YELLOW; \
    yeti_log_data.filename = __FILE__; \
    yeti_log_data.funcname = __func__; \
    yeti_log_data.line = __LINE__; \
    yeti_log_data.msg_id = yeti_lot_msg_id; \
    \
    char yeti_log_msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(yeti_log_msg, sizeof(yeti_log_msg), fmt, ##__VA_ARGS__); \
    yeti_log_data.msg = yeti_log_msg; \
    \
    yeti::_EnqueueLogTask(&yeti_log_data); \
  } \
}

/**
 * @brief Logs informational message using specified printf-like format.
 */
#define INFO(fmt, ...) { \
  std::size_t yeti_lot_msg_id = yeti::_GetMsgId(); \
  yeti::_IncMsgId(); \
  if (yeti::GetLogLevel() >= yeti::LOG_LEVEL_INFO) { \
    yeti::LogData yeti_log_data; \
    yeti_log_data.level = "INFO"; \
    yeti_log_data.color = YETI_LGREEN; \
    yeti_log_data.filename = __FILE__; \
    yeti_log_data.funcname = __func__; \
    yeti_log_data.line = __LINE__; \
    yeti_log_data.msg_id = yeti_lot_msg_id; \
    \
    char yeti_log_msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(yeti_log_msg, sizeof(yeti_log_msg), fmt, ##__VA_ARGS__); \
    yeti_log_data.msg = yeti_log_msg; \
    \
    yeti::_EnqueueLogTask(&yeti_log_data); \
  } \
}

/**
 * @brief Logs debug message using specified printf-like format.
 */
#define DEBUG(fmt, ...) { \
  std::size_t yeti_lot_msg_id = yeti::_GetMsgId(); \
  yeti::_IncMsgId(); \
  if (yeti::GetLogLevel() >= yeti::LOG_LEVEL_DEBUG) { \
    yeti::LogData yeti_log_data; \
    yeti_log_data.level = "DEBUG"; \
    yeti_log_data.color = YETI_WHITE; \
    yeti_log_data.filename = __FILE__; \
    yeti_log_data.funcname = __func__; \
    yeti_log_data.line = __LINE__; \
    yeti_log_data.msg_id = yeti_lot_msg_id; \
    \
    char yeti_log_msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(yeti_log_msg, sizeof(yeti_log_msg), fmt, ##__VA_ARGS__); \
    yeti_log_data.msg = yeti_log_msg; \
    \
    yeti::_EnqueueLogTask(&yeti_log_data); \
  } \
}

/**
 * @brief Logs trace message using specified printf-like format.
 */
#define TRACE(fmt, ...) { \
  std::size_t yeti_lot_msg_id = yeti::_GetMsgId(); \
  yeti::_IncMsgId(); \
  if (yeti::GetLogLevel() >= yeti::LOG_LEVEL_TRACE) { \
    yeti::LogData yeti_log_data; \
    yeti_log_data.level = "TRACE"; \
    yeti_log_data.color = ""; \
    yeti_log_data.filename = __FILE__; \
    yeti_log_data.funcname = __func__; \
    yeti_log_data.line = __LINE__; \
    yeti_log_data.msg_id = yeti_lot_msg_id; \
    \
    char yeti_log_msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(yeti_log_msg, sizeof(yeti_log_msg), fmt, ##__VA_ARGS__); \
    yeti_log_data.msg = yeti_log_msg; \
    \
    yeti::_EnqueueLogTask(&yeti_log_data); \
  } \
}

#endif  // DISABLE_LOGGING

#endif  // INC_YETI_MACRO_H_
