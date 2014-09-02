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

#include <iostream>
#include <yeti/yeti.h>

#ifdef _WIN32
#  define TMP_DIR "C:\\Temp"
#else
#  define TMP_DIR "/tmp"
#endif  // _WIN32


void TestLog(yeti::LogLevel level) {
  yeti::SetLevel(level);
  TRACE("trace info: function trace");
  DEBUG("debug output %s", "test debug output");
  INFO("info output sizeof(int) = %zu", sizeof(int));
  WARN("warning output: %s", "test text");
  ERROR("error output: %f", 2.5F);
  CRITICAL("critical output: %d\n", 0);
}


int main(int argc, char* argv[]) {
  yeti::SetColored(true);
  TestLog(yeti::LOG_LEVEL_TRACE);
  TestLog(yeti::LOG_LEVEL_DEBUG);
  TestLog(yeti::LOG_LEVEL_INFO);

  yeti::SetColored(false);
  FILE* fd = ::fopen(TMP_DIR "/output_test.log", "w");
  yeti::SetFileDesc(fd);
  TestLog(yeti::LOG_LEVEL_WARNING);
  TestLog(yeti::LOG_LEVEL_ERROR);
  TestLog(yeti::LOG_LEVEL_CRITICAL);
  yeti::CloseFile();

  return 0;
}
