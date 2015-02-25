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

#include <chrono>
#include <iostream>
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


TEST(YETI, PERFORMANCE_TEST) {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  int logged_msg = 100000;
  for (int i = 0; i < logged_msg / 6; ++i) {
    ShowSimpleTestMsg();
  }
  yeti::FlushLog();

  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_time = end - start;
  std::cerr << "elapsed time to print " << logged_msg << ": "
            << elapsed_time.count() << " sec" << std::endl;
}
