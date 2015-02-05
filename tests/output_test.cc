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


void TestLog() {
  TRACE("some trace info");

  std::string debug_str = "test string";
  DEBUG("print string: %s", debug_str.c_str());

  INFO("is current log colored? %d", yeti::IsLogColored());
  WARN("current file descriptor: %d", fileno(yeti::GetLogFileDesc()));

  int array[3] = { 0, 1, 2 };
  ERROR("print array elements: (%d, %d, %d)", array[0], array[1], array[2]);

  CRITICAL("current log level: %d\n", yeti::GetLogLevel());
}


int main(int argc, char* argv[]) {
  TestLog();

  yeti::SetLogColored(true);  // turn on log colorization
  yeti::SetLogLevel(yeti::LOG_LEVEL_TRACE);
  TestLog();

  yeti::SetLogFormatStr(
      "[%(LEVEL)] <%(DATE) %(TIME)> %(FILENAME): %(LINE): %(MSG)");
  yeti::SetLogLevel(yeti::LOG_LEVEL_DEBUG);
  TestLog();

  yeti::SetLogColored(false);  // turn off log colorization
  yeti::SetLogFormatStr(
      "%(MSG_ID) [%(LEVEL)] <%(TID)> %(FILENAME): %(LINE): %(MSG)");
  yeti::SetLogLevel(yeti::LOG_LEVEL_INFO);
  TestLog();

  FILE* fd = std::fopen("/tmp/test.log", "w");
  yeti::SetLogFileDesc(fd);  // start logging into specified file
  yeti::SetLogLevel(yeti::LOG_LEVEL_WARNING);
  TestLog();
  yeti::SetLogFormatStr("[%(LEVEL)] [%(PID):%(TID)] %(FUNCNAME)(): %(MSG)");
  yeti::SetLogLevel(yeti::LOG_LEVEL_ERROR);
  TestLog();
  yeti::SetLogColored(true);
  yeti::SetLogLevel(yeti::LOG_LEVEL_CRITICAL);
  TestLog();
  yeti::CloseLogFileDesc();  // enqueue closing file descriptor

  return 0;
}
