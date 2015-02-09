// Copyright (c) 2014-2015, Dmitry Senin (seninds@gmail.com)
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

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <yeti/yeti.h>


void ShowTestMsg() {
  const std::map<int, std::string> level_names = {
      {yeti::LOG_LEVEL_CRITICAL, "CRITICAL"},
      {yeti::LOG_LEVEL_ERROR, "ERROR"},
      {yeti::LOG_LEVEL_WARNING, "WARNING"},
      {yeti::LOG_LEVEL_INFO, "INFO"},
      {yeti::LOG_LEVEL_DEBUG, "DEBUG"},
      {yeti::LOG_LEVEL_TRACE, "TRACE"}
  };

  CRIT("Selected log level: %d (%s)", yeti::GetLogLevel(),
           level_names.at(yeti::GetLogLevel()).c_str());

  CRIT("test critical msg");
  ERR("test error msg");
  WRN("test warning msg");
  INFO("test info msg");
  DBG("test debug msg");
  TRACE("test trace msg");
}


TEST(YETI, YETI_LOG_LEVEL) {
  struct LogLevel {
    int id;
    std::string name;
  };

  const std::vector<LogLevel> env_vars = {
      {yeti::LOG_LEVEL_CRITICAL, "crt"},
      {yeti::LOG_LEVEL_ERROR, "err"},
      {yeti::LOG_LEVEL_WARNING, "wrn"},
      {yeti::LOG_LEVEL_INFO, "inf"},
      {yeti::LOG_LEVEL_DEBUG, "dbg"},
      {yeti::LOG_LEVEL_TRACE, "trc"}
  };

  std::srand(std::time(0));
  LogLevel selected_level = env_vars[std::rand() % env_vars.size()];
  setenv("YETI_LOG_LEVEL", selected_level.name.c_str(), 1);
  EXPECT_EQ(selected_level.id, yeti::GetLogLevel());

  char buffer[4096] = { 0 };
  setbuffer(stderr, buffer, sizeof(buffer));
  ShowTestMsg();
  yeti::FlushLog();
  EXPECT_EQ(selected_level.id + 2,
            std::count(buffer, buffer + sizeof(buffer), '\n'));
}
