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
#include <cstring>
#include <ctime>

#include <algorithm>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <yeti/yeti.h>


void ShowSimpleTestMsg() {
  CRIT("test critical msg");
  ERR("test error msg");
  WARN("test warning msg");
  INFO("test info msg");
  DBG("test debug msg");
  TRACE("test trace msg");
}


std::vector<yeti::LogLevel> g_levels = {
  yeti::LOG_LEVEL_CRITICAL, yeti::LOG_LEVEL_ERROR,
  yeti::LOG_LEVEL_WARNING, yeti::LOG_LEVEL_INFO,
  yeti::LOG_LEVEL_DEBUG, yeti::LOG_LEVEL_TRACE
};


TEST(YETI, SET_LOG_LEVEL) {
  char buffer[4096] = { 0 };
  setvbuf(stderr, buffer, _IOFBF, sizeof(buffer));

  int current_str_count = 0;
  int expected_str_count = 0;
  for (const auto& level : g_levels) {
    yeti::SetLogLevel(level);
    ShowSimpleTestMsg();
    expected_str_count += level + 1;
    yeti::FlushLog();
    current_str_count = std::count(buffer, buffer + sizeof(buffer), '\n');
    EXPECT_EQ(expected_str_count, current_str_count);
  }
}

TEST(YETI, GET_LOG_LEVEL) {
  for (const auto& level : g_levels) {
    yeti::SetLogLevel(level);
    EXPECT_EQ(level, yeti::GetLogLevel());
  }
}

TEST(YETI, IS_LOG_COLORED) {
  yeti::SetLogColored(true);
  EXPECT_TRUE(yeti::IsLogColored());
  yeti::SetLogColored(false);
  EXPECT_FALSE(yeti::IsLogColored());
}
