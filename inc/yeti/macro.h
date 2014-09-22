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

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wformat-security"
#elif defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic ignored "-Wformat-security"
#endif  // ignored "-Wformat-security"

#define MAX_MSG_LENGTH 512

#include <yeti/color.h>
#include <yeti/log_data.h>

namespace yeti {

void _EnqueueLogTask(yeti::LogData* log_data);

}  // namespace yeti


#define CRITICAL(fmt, ...) { \
  std::size_t msg_id = yeti::Logger::instance().GetMsgId(); \
  yeti::Logger::instance().IncMsgId(); \
  char msg[MAX_MSG_LENGTH] = { 0 }; \
  std::snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
  yeti::LogData data { .level = "CRITICAL", .color = YETI_LRED, .filename = __FILE__, \
                       .funcname = __func__, .msg = msg, .line = __LINE__, \
                       .msg_id = msg_id }; \
  yeti::_EnqueueLogTask(&data); \
}

#define ERROR(fmt, ...) { \
  std::size_t msg_id = yeti::Logger::instance().GetMsgId(); \
  yeti::Logger::instance().IncMsgId(); \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_ERROR) { \
    char msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
    yeti::LogData data { .level = "ERROR", .color = YETI_LPURPLE, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg = msg, .line = __LINE__, .msg_id = msg_id }; \
    yeti::_EnqueueLogTask(&data); \
  } \
}

#define WARN(fmt, ...) { \
  std::size_t msg_id = yeti::Logger::instance().GetMsgId(); \
  yeti::Logger::instance().IncMsgId(); \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_WARNING) { \
    char msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
    yeti::LogData data { .level = "WARNING", .color = YETI_YELLOW, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg = msg, .line = __LINE__, .msg_id = msg_id }; \
    yeti::_EnqueueLogTask(&data); \
  } \
}

#define INFO(fmt, ...) { \
  std::size_t msg_id = yeti::Logger::instance().GetMsgId(); \
  yeti::Logger::instance().IncMsgId(); \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_INFO) { \
    char msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
    yeti::LogData data { .level = "INFO", .color = YETI_LGREEN, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg = msg, .line = __LINE__, .msg_id = msg_id }; \
    yeti::_EnqueueLogTask(&data); \
  } \
}

#define DEBUG(fmt, ...) { \
  std::size_t msg_id = yeti::Logger::instance().GetMsgId(); \
  yeti::Logger::instance().IncMsgId(); \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_DEBUG) { \
    char msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
    yeti::LogData data { .level = "DEBUG", .color = YETI_WHITE, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg = msg, .line = __LINE__, .msg_id = msg_id }; \
    yeti::_EnqueueLogTask(&data); \
  } \
}

#define TRACE(fmt, ...) { \
  std::size_t msg_id = yeti::Logger::instance().GetMsgId(); \
  yeti::Logger::instance().IncMsgId(); \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_TRACE) { \
    char msg[MAX_MSG_LENGTH] = { 0 }; \
    std::snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__); \
    yeti::LogData data { .level = "TRACE", .color = "", \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg = msg, .line = __LINE__, .msg_id = msg_id }; \
    yeti::_EnqueueLogTask(&data); \
  } \
}

#endif  // INC_YETI_MACRO_H_
