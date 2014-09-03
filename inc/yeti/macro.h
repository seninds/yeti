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
#include <thread>

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wformat-security"
#elif defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic ignored "-Wformat-security"
#endif  // ignored "-Wformat-security"

template <typename... Args>
void _CopyArgs(const std::string& tag_str, const std::string& filename,
               const std::string& func_name, int line,
               const std::string& msg_fmt, Args... args);

#define CRITICAL(msg_fmt, ...) { \
  std::string tag_str = "CRITICAL"; \
  std::string tag_color = YETI_LRED; \
  std::string filename = __FILE__; \
  std::string func_name(__func__); \
  std::string fmt = msg_fmt; \
  int line = __LINE__; \
  _CopyArgs(tag_str, tag_color, filename, func_name, line, fmt, ##__VA_ARGS__); \
}

#define ERROR(msg_fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_ERROR) { \
    std::string tag_str = "ERROR"; \
    std::string tag_color = YETI_LPURPLE; \
    std::string filename = __FILE__; \
    std::string func_name(__func__); \
    std::string fmt = msg_fmt; \
    int line = __LINE__; \
    _CopyArgs(tag_str, tag_color, filename, func_name, line, fmt, ##__VA_ARGS__); \
  } \
}

#define WARN(msg_fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_WARNING) { \
    std::string tag_str = "WARNING"; \
    std::string tag_color = YETI_YELLOW; \
    std::string filename = __FILE__; \
    std::string func_name(__func__); \
    std::string fmt = msg_fmt; \
    int line = __LINE__; \
    _CopyArgs(tag_str, tag_color, filename, func_name, line, fmt, ##__VA_ARGS__); \
  } \
}

#define INFO(msg_fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_INFO) { \
    std::string tag_str = "INFO"; \
    std::string tag_color = YETI_LGREEN; \
    std::string filename = __FILE__; \
    std::string func_name(__func__); \
    std::string fmt = msg_fmt; \
    int line = __LINE__; \
    _CopyArgs(tag_str, tag_color, filename, func_name, line, fmt, ##__VA_ARGS__); \
  } \
}

#define DEBUG(msg_fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_DEBUG) { \
    std::string tag_str = "DEBUG"; \
    std::string tag_color = YETI_WHITE; \
    std::string filename = __FILE__; \
    std::string func_name(__func__); \
    std::string fmt = msg_fmt; \
    int line = __LINE__; \
    _CopyArgs(tag_str, tag_color, filename, func_name, line, fmt, ##__VA_ARGS__); \
  } \
}

#define TRACE(msg_fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_TRACE) { \
    std::string tag_str = "TRACE"; \
    std::string tag_color = ""; \
    std::string filename = __FILE__; \
    std::string func_name(__func__); \
    std::string fmt = msg_fmt; \
    int line = __LINE__; \
    _CopyArgs(tag_str, tag_color, filename, func_name, line, fmt, ##__VA_ARGS__); \
  } \
}

std::string ParseFormatStr(
    const std::string format_str, const std::string& tag_str,
    const std::string& filename, const std::string& func_name, pid_t pid,
    std::thread::id tid, int line, const std::string& msg_fmt);

template <typename... Args>
void _CopyArgs(const std::string& tag_str, const std::string& tag_color,
               const std::string& filename, const std::string& func_name,
               int line, const std::string& msg_fmt, Args ... args) {
  std::string format_str = yeti::Logger::instance().GetFormatStr();
  pid_t pid = getpid();
  std::thread::id tid = std::this_thread::get_id();
  FILE *fd = yeti::Logger::instance().GetFileDesc();
  bool is_colored = yeti::Logger::instance().GetColored();

  auto print_func = [format_str, tag_str, tag_color, filename, func_name, pid,
                     tid, line, fd, is_colored, msg_fmt, args...] {
    std::string final_fmt = ParseFormatStr(format_str, tag_str, filename,
                                           func_name, pid, tid, line, msg_fmt)
                            + "\n";
    if (isatty(fileno(fd)) != 0 && is_colored) {
      final_fmt = tag_color + final_fmt + std::string(YETI_RESET);
    }
    std::fprintf(fd, final_fmt.c_str(), args...);
  };
  yeti::Logger::instance().PushToLog(print_func);
}

inline std::string ParseFormatStr(
    const std::string format_str, const std::string& tag_str,
    const std::string& filename, const std::string& func_name, pid_t pid,
    std::thread::id tid, int line, const std::string& msg_fmt) {
  std::map<std::string, std::string> subs;
  subs["%(TAG)"] = tag_str;
  subs["%(FILENAME)"] = filename;
  subs["%(FUNCNAME)"] = func_name;
  subs["%(PID)"] = std::to_string(pid);

  std::hash<std::thread::id> hash_fn;
  std::ostringstream oss;
  oss << std::hex << std::uppercase << hash_fn(tid);
  subs["%(TID)"] = oss.str();

  subs["%(LINE)"] = std::to_string(line);
  subs["%(MSG)"] = msg_fmt;

  std::string result = format_str;
  size_t pos = 0;
  for (const auto& entry : subs) {
    while ((pos = result.find(entry.first)) != std::string::npos) {
      result.replace(pos, entry.first.length(), entry.second);
    }
  }
  return result;
}

#endif  // INC_YETI_MACRO_H_
