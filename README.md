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
yeti::SetLogFormatStr("[%(TAG)] [%(PID)] %(FILENAME): %(LINE): %(MSG)");
~~~~~~

Keywords to set log format are:
* %(TAG)      - tag of current logging level
* %(FILENAME) - filename
* %(FUNCNAME) - function name
* %(PID)      - process ID
* %(TID)      - thread ID
* %(LINE)     - line number
* %(MSG)      - user message (format string)
* %(MSG_ID)   - unique message number

## Usage

To use __Yeti__ you should:
* add content of _"inc/yeti"_ to your project;
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

* clang v3.5 or later
* pthread
* scons v2.3.3 or later

__Yeti__ uses C++11 and multithreading, so you should add *-std=c++11* and *-pthread* compile and link options.
__Yeti__ uses SCons as a build system. So to build project and run *output_test* just type in your console:

~~~~~~
# scons
# LD_LIBRARY_PATH="./build" ./build/tests/output_test
~~~~~~

GCC 4.8.2 has a bug with lambda
(<a href="https://gcc.gnu.org/bugzilla/show_bug.cgi?id=41933">lambda
doesn't capture parameter pack</a>),
so you have to use clang (v3.5 or later) to use __Yeti__. Hope
guys from GNU Compiler team will fix this bug in the nearest future.


## Example

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
  TestLog(yeti::LOG_LEVEL_DEBUG);

  yeti::SetLogColored(false);  // turn off log colorization
  yeti::SetLogFormatStr(
      "%(MSG_ID) [%(TAG)] [PID:%(PID)] %(FILENAME): %(LINE): %(MSG)");
  TestLog(yeti::LOG_LEVEL_INFO);

  FILE* fd = std::fopen("/tmp/test.log", "w");
  yeti::SetLogFileDesc(fd);  // start logging into specified file
  TestLog(yeti::LOG_LEVEL_WARNING);
  yeti::SetLogFormatStr("[%(TAG)] [%(PID):%(TID)] %(FUNCNAME)(): %(MSG)");
  TestLog(yeti::LOG_LEVEL_ERROR);
  yeti::SetLogColored(true);
  TestLog(yeti::LOG_LEVEL_CRITICAL);
  yeti::CloseLogFileDesc();  // enqueue closing file descriptor

  return 0;
}
~~~~~~

## LICENCE

__Yeti__ is distributed with a BSD license
