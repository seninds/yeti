Yeti: C++ lightweight threadsafe logging
========================================

__Yeti__ is lightweight threadsafe logging, which is running in separate thread,
so it can't slow your application. 

It has 6 logging levels:
~~~~~~
enum LogLevel {
  LOG_LEVEL_CRITICAL,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_TRACE
}
~~~~~~

Logging has printf-style compact format:
~~~~~~
ERROR("[%d] exception: %s", function_id, e.what());
~~~~~~

You can tune log format:
~~~~~~
yeti::SetLogFormatStr("[%(LEVEL)] [%(PID)] %(FILENAME): %(LINE): %(MSG)");
~~~~~~

Keywords to set log format are:
* %(LEVEL)    - logging level
* %(FILENAME) - filename
* %(FUNCNAME) - function name
* %(PID)      - process ID
* %(TID)      - thread ID
* %(LINE)     - line number
* %(MSG)      - user message
* %(MSG_ID)   - unique message number to refer in discussion with colleagues
* %(DATE)     - local date in YYYY-MM-DD format (the ISO 8601 date format)
* %(TIME)     - local time in HH:MM:SS.SSS format (based on the ISO 8601 time format)

## Usage

To use __Yeti__ you should:
* add content from _"inc/yeti"_ to your project;
* add __#include <yeti/yeti.h>__ into your source files;
* build __yeti__ lib and link with it your project.

Logging has a lazy initialization, so if you don't use it you will not have any
overhead. Just add into your code following macros:
~~~~~~
CRITICAL(msg_fmt, ...);
ERROR(msg_fmt, ...);
WARN(msg_fmt, ...);
INFO(msg_fmt, ...);
DEBUG(msg_fmt, ...);
TRACE(msg_fmt, ...);
~~~~~~

Logging control is based on yeti namespace functions:
~~~~~~
namespace yeti {
  void SetLogLevel(LogLevel level) noexcept;
  int GetLogLevel() noexcept;
  void SetLogColored(bool is_colored) noexcept;
  bool IsLogColored() noexcept;
  void SetLogFileDesc(FILE* fd) noexcept;
  FILE* GetLogFileDesc() noexcept;
  void CloseLogFileDesc(FILE* fd = nullptr);
  void SetLogFormatStr(const std::string& format_str) noexcept;
  std::string GetLogFormatStr() noexcept;
}
~~~~~~

## Requirements

* clang v3.4 or later, gcc v4.8 or later;
* pthread
* scons v2.3.3 or later

__Yeti__ uses C++11 and multithreading, so you should add *-std=c++11* and *-pthread* compile and link options.
__Yeti__ uses SCons as a build system. So to build project and run *output_test* just type in your console:

~~~~~~
# scons
# LD_LIBRARY_PATH="./build" ./build/tests/output_test
~~~~~~

## Example

__Example to test Yeti output:__
~~~~~~
#include <yeti/yeti.h>


void TestLog(yeti::LogLevel level) {
  yeti::SetLogLevel(level);
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
  yeti::SetLogColored(true);  // turn on log colorization
  TestLog(yeti::LOG_LEVEL_TRACE);
  yeti::SetLogFormatStr(
      "[%(LEVEL)] <%(DATE) %(TIME)> %(FILENAME): %(LINE): %(MSG)");
  TestLog(yeti::LOG_LEVEL_DEBUG);

  yeti::SetLogColored(false);  // turn off log colorization
  yeti::SetLogFormatStr(
      "%(MSG_ID) [%(LEVEL)] <%(TID)> %(FILENAME): %(LINE): %(MSG)");
  TestLog(yeti::LOG_LEVEL_INFO);

  FILE* fd = std::fopen("/tmp/test.log", "w");
  yeti::SetLogFileDesc(fd);  // start logging into specified file
  TestLog(yeti::LOG_LEVEL_WARNING);
  yeti::SetLogFormatStr("[%(LEVEL)] [%(PID):%(TID)] %(FUNCNAME)(): %(MSG)");
  TestLog(yeti::LOG_LEVEL_ERROR);
  yeti::SetLogColored(true);
  TestLog(yeti::LOG_LEVEL_CRITICAL);
  yeti::CloseLogFileDesc();  // enqueue closing file descriptor

  return 0;
}
~~~~~~

__OUTPUT: stderr__
~~~~~~
[TRACE] tests/output_test.cc: 33: some trace info
[DEBUG] tests/output_test.cc: 36: print string: test string
[INFO] tests/output_test.cc: 38: is current log colored? 1
[WARNING] tests/output_test.cc: 39: current file descriptor: 2
[ERROR] tests/output_test.cc: 42: print array elements: (0, 1, 2)
[CRITICAL] tests/output_test.cc: 44: current log level: 5

[DEBUG] <2014-09-22 16:35:46.367622886> tests/output_test.cc: 36: print string: test string
[INFO] <2014-09-22 16:35:46.367625076> tests/output_test.cc: 38: is current log colored? 1
[WARNING] <2014-09-22 16:35:46.367627078> tests/output_test.cc: 39: current file descriptor: 2
[ERROR] <2014-09-22 16:35:46.367629321> tests/output_test.cc: 42: print array elements: (0, 1, 2)
[CRITICAL] <2014-09-22 16:35:46.368124046> tests/output_test.cc: 44: current log level: 4

14 [INFO] <3642E50D01F30105> tests/output_test.cc: 38: is current log colored? 0
15 [WARNING] <3642E50D01F30105> tests/output_test.cc: 39: current file descriptor: 2
16 [ERROR] <3642E50D01F30105> tests/output_test.cc: 42: print array elements: (0, 1, 2)
17 [CRITICAL] <3642E50D01F30105> tests/output_test.cc: 44: current log level: 3
~~~~~~

__OUTPUT: /tmp/test.log__
~~~~~~
21 [WARNING] <3642E50D01F30105> tests/output_test.cc: 39: current file descriptor: 3
22 [ERROR] <3642E50D01F30105> tests/output_test.cc: 42: print array elements: (0, 1, 2)
23 [CRITICAL] <3642E50D01F30105> tests/output_test.cc: 44: current log level: 2

[ERROR] [12446:3642E50D01F30105] TestLog(): print array elements: (0, 1, 2)
[CRITICAL] [12446:3642E50D01F30105] TestLog(): current log level: 1

[CRITICAL] [12446:3642E50D01F30105] TestLog(): current log level: 0
~~~~~~

## LICENCE

__Yeti__ is distributed with a BSD license
