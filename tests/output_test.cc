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

#include <yeti/yeti.h>

void TestLog(yeti::LogLevel level) {
  yeti::SetLevel(level);  // set current log level
  TRACE("trace info: function trace");
  DEBUG("debug output %s", "test debug output");
  DEBUG("debug simple output");
  INFO("info output sizeof(int) = %zu", sizeof(int));
  WARN("warning output: %s", "test text");
  ERROR("error output: %f", 2.5F);
  CRITICAL("critical output: %d\n", 0);
}


int main(int argc, char* argv[]) {
  yeti::SetColored(true);  // turn on log colorization
  TestLog(yeti::LOG_LEVEL_TRACE);
  TestLog(yeti::LOG_LEVEL_DEBUG);

  yeti::SetColored(false);  // turn off log colorization
  yeti::SetFormatStr("[%(TAG)] [%(PID)] %(FILENAME): %(LINE): %(MSG)");
  TestLog(yeti::LOG_LEVEL_INFO);

  FILE* fd = ::fopen("output_test.log", "w");
  yeti::SetFileDesc(fd);  // start logging into specified file
  TestLog(yeti::LOG_LEVEL_WARNING);
  yeti::SetFormatStr("[%(TAG)] [%(PID):%(TID)] %(FUNCNAME)(): %(MSG)");
  TestLog(yeti::LOG_LEVEL_ERROR);
  yeti::SetColored(true);
  TestLog(yeti::LOG_LEVEL_CRITICAL);  // check is a tty device currently using
  yeti::CloseFile();

  return 0;
}
