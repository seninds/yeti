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

#ifndef INC_YETI_MACRO_H_
#define INC_YETI_MACRO_H_

#include <unistd.h>
#include <cstdio>
#include <sstream>
#include <string>
#include <thread>

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wformat-security"
#elif defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic ignored "-Wformat-security"
#endif  // ignored "-Wformat-security"


struct LogData {
  std::string log_format;
  std::string tag;
  std::string color;
  std::string filename;
  std::string funcname;
  std::string msg_format;
  pid_t pid;
  std::thread::id tid;
  int line;
  FILE* fd;
};


template <typename... Args>
void _CopyArgs(LogData& log_data, Args... args);


#define CRITICAL(fmt, ...) { \
  LogData data { .tag = "CRITICAL", .color = YETI_LRED, .filename = __FILE__, \
                 .funcname = __func__, .msg_format = fmt, .line = __LINE__ }; \
  _CopyArgs(data, ##__VA_ARGS__); \
}

#define ERROR(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_ERROR) { \
    LogData data { .tag = "ERROR", .color = YETI_LPURPLE, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg_format = fmt, .line = __LINE__ }; \
    _CopyArgs(data, ##__VA_ARGS__); \
  } \
}

#define WARN(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_WARNING) { \
    LogData data { .tag = "WARNING", .color = YETI_YELLOW, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg_format = fmt, .line = __LINE__ }; \
    _CopyArgs(data, ##__VA_ARGS__); \
  } \
}

#define INFO(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_INFO) { \
    LogData data { .tag = "INFO", .color = YETI_LGREEN, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg_format = fmt, .line = __LINE__ }; \
    _CopyArgs(data, ##__VA_ARGS__); \
  } \
}

#define DEBUG(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_DEBUG) { \
    LogData data { .tag = "DEBUG", .color = YETI_WHITE, \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg_format = fmt, .line = __LINE__ }; \
    _CopyArgs(data, ##__VA_ARGS__); \
  } \
}

#define TRACE(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_TRACE) { \
    LogData data { .tag = "TRACE", .color = "", \
                   .filename = __FILE__, .funcname = __func__, \
                   .msg_format = fmt, .line = __LINE__ }; \
    _CopyArgs(data, ##__VA_ARGS__); \
  } \
}

std::string ParseFormatStr(const LogData& log_data);

template <typename... Args>
void _CopyArgs(LogData& log_data, Args ... args) {
  log_data.log_format = yeti::Logger::instance().GetFormatStr();
  log_data.pid = getpid();
  log_data.tid = std::this_thread::get_id();
  log_data.fd = yeti::Logger::instance().GetFileDesc();
  bool is_colored = yeti::Logger::instance().GetColored();

  auto print_func = [log_data, is_colored, args...] {
    std::string final_fmt = ParseFormatStr(log_data) + "\n";
    if (isatty(fileno(log_data.fd)) != 0 && is_colored) {
      final_fmt = log_data.color + final_fmt + std::string(YETI_RESET);
    }
    std::fprintf(log_data.fd, final_fmt.c_str(), args...);
  };

  yeti::Logger::instance().PushToLog(print_func);
}

inline std::string ParseFormatStr(const LogData& log_data) {
  std::map<std::string, std::string> subs;
  subs["%(TAG)"] = log_data.tag;
  subs["%(FILENAME)"] = log_data.filename;
  subs["%(FUNCNAME)"] = log_data.funcname;
  subs["%(PID)"] = std::to_string(log_data.pid);

  std::hash<std::thread::id> hash_fn;
  std::ostringstream oss;
  oss << std::hex << std::uppercase << hash_fn(log_data.tid);
  subs["%(TID)"] = oss.str();

  subs["%(LINE)"] = std::to_string(log_data.line);
  subs["%(MSG)"] = log_data.msg_format;

  std::string result = log_data.log_format;
  size_t pos = 0;
  for (const auto& entry : subs) {
    while ((pos = result.find(entry.first)) != std::string::npos) {
      result.replace(pos, entry.first.length(), entry.second);
    }
  }
  return result;
}

#endif  // INC_YETI_MACRO_H_
