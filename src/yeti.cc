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

#include <yeti/yeti.h>

#include <csignal>
#include <cstdlib>
#include <map>
#include <mutex>
#include <string>
#include <yeti/logger.h>

namespace yeti {

std::map<int, std::string> SIGNAME = {
    { SIGABRT, "SIGABRT" },
    { SIGFPE, "SIGFPE" },
    { SIGILL, "SIGILL" },
    { SIGINT, "SIGINT" },
    { SIGSEGV, "SIGSEGV" },
    { SIGTERM, "SIGTERM" }
};

void ShutdownLog() {
  yeti::Logger::instance().Shutdown();
}

void SignalHandler(int sig_num) {
  std::fprintf(stderr, "caught %s: start flushing log...\n",
               SIGNAME[sig_num].c_str());
  std::exit(sig_num);
}

void RegisterSignals() {
  std::atexit(ShutdownLog);
  for (const auto& entry : SIGNAME) {
    signal(entry.first, SignalHandler);
  }
}

std::string _CreateLogStr(const yeti::LogData& log_data) {
  std::map<std::string, std::string> subs;
  subs["%(LEVEL)"] = log_data.level;
  subs["%(FILENAME)"] = log_data.filename;
  subs["%(FUNCNAME)"] = log_data.funcname;
  subs["%(PID)"] = std::to_string(log_data.pid);

  std::hash<std::thread::id> hash_fn;
  std::ostringstream oss;
  oss << std::hex << std::uppercase << hash_fn(log_data.tid);
  subs["%(TID)"] = oss.str();

  subs["%(LINE)"] = std::to_string(log_data.line);
  subs["%(MSG)"] = log_data.msg;
  subs["%(MSG_ID)"] = std::to_string(log_data.msg_id);

  std::string result = log_data.log_format;
  size_t pos = 0;
  for (const auto& entry : subs) {
    while ((pos = result.find(entry.first)) != std::string::npos) {
      result.replace(pos, entry.first.length(), entry.second);
    }
  }
  return result;
}

void _EnqueueLogTask(LogData* log_data) {
  log_data->log_format = yeti::Logger::instance().GetFormatStr();
  log_data->pid = getpid();
  log_data->tid = std::this_thread::get_id();
  log_data->fd = yeti::Logger::instance().GetFileDesc();
  bool is_colored = yeti::Logger::instance().IsColored();

  LogData printed_data = *log_data;
  auto print_func = [printed_data, is_colored] {
    std::string log_str = _CreateLogStr(printed_data) + "\n";
    if (isatty(fileno(printed_data.fd)) != 0 && is_colored) {
      log_str = printed_data.color + log_str + std::string(YETI_RESET);
    }
    std::fprintf(printed_data.fd, log_str.c_str());
  };

  yeti::Logger::instance().EnqueueTask(print_func);
}

}  // namespace yeti
