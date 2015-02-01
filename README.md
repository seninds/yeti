Yeti: C++ lightweight threadsafe logging
========================================

**Yeti** is C++ lightweight threadsafe logging system,
which is running in separate thread, so it can't slow your application. 

**Yeti** has a lazy initialization, so if you don't use it you will not have any
overhead. Just add into your code following macros to log something:
~~~~~~
CRITICAL(msg_fmt, ...);
ERROR(msg_fmt, ...);
WARN(msg_fmt, ...);
INFO(msg_fmt, ...);
DEBUG(msg_fmt, ...);
TRACE(msg_fmt, ...);
~~~~~~


## Usage ##

To use **Yeti** you should:
* add headers from **"inc/yeti"** to your project;
* add **#include \<yeti/yeti.h\>** into your source files;
* **build** yeti lib and **link** with it your project.

### Set Log Level ###

**Yeti** has 6 log levels:
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

You can change log level at runtime using
*yeti::SetLogLevel(yeti::LogLevel)* function.

You can also set log level by environment variable *YETI_LOG_LEVEL*.
All most popular variants of log level naming is taking into account,
so you shouldn't remember the correct level name.

**Yeti** sets corresponding log level if *YETI_LOG_LEVEL* contains one
of the following substrings:

| Log Level           | Substrings                      |
|---------------------|---------------------------------|
| LOG_LEVEL_TRACE     | "TRACE", "TRC", "trace", "trc"  |
| LOG_LEVEL_DEBUG     | "DEBUG", "DBG", "debug", "dbg"  |
| LOG_LEVEL_INFO      | "INF",   "inf"                  |
| LOG_LEVEL_WARNING   | "WARN",  "WRN", "warn",  "wrn"  |
| LOG_LEVEL_ERROR     | "ERR",   "err"                  |
| LOG_LEVEL_CRITICAL  | "CRIT",  "CRT", "crit",  "crt"  |

Example of setting log level using environment variable *YETI_LOG_LEVEL*:
~~~~~~
YETI_LOG_LEVEL=dbg ./build/test_app
~~~~~~

### Set Log Format ###

Logging has printf-style compact format:
~~~~~~
ERROR("[%d] exception: %s", function_id, e.what());
~~~~~~

You can tune log format using *yeti::SetLogFormatStr(format_str)* function:
~~~~~~
yeti::SetLogFormatStr("[%(LEVEL)] [%(PID)] %(FILENAME): %(LINE): %(MSG)");
~~~~~~

Keywords to set log format are:

| Keyword     | Description                                                           |
|-------------|-----------------------------------------------------------------------|
| %(LEVEL)    | logging level                                                         |
| %(FILENAME) | filename                                                              |
| %(FUNCNAME) | function name                                                         |
| %(PID)      | process ID                                                            |
| %(TID)      | thread ID                                                             |
| %(LINE)     | line number                                                           |
| %(MSG)      | user message                                                          |
| %(MSG_ID)   | unique message number to refer in discussion with colleagues          |
| %(DATE)     | local date in YYYY-MM-DD format (the ISO 8601 date format)            |
| %(TIME)     | local time in HH:MM:SS.SSS format (based on the ISO 8601 time format) |


### Handle Signals ###

**Yeti** process runs in separate thread, so then some signals (for example, SIGABRT)
has been generated **Yeti** could have non-empty message queue.
If you don't flush **Yeti** message queue you lose these messages.

To flush message queue you can use
*yeti::RegAllSignals(__sighandler_t = SimpleSignalHandler)* or
*yeti::RegSignal(sig_num, __sighandler_t = SimpleSignalHandler)* funcions with
default value for signal handler (this function only flushing message queue).
You also can set custom signal handler (its signature should be
*void signal_handler(int)* where you can use *yeti::FlushLog()* function
to flush message queue by hands.

Example to set signal handlers:
~~~~~~
#include <cstdlib>
#include <yeti/yeti.h>

int main() {
  yeti::RegAllSignals();
  // code with logging...
  std::abort();
  // code with logging...
  return 0;
}
~~~~~~


### Disable Logging ###

If you want to test your application (for example, for profiling) without logging
you should add *DISABLE_LOGGING* definition before including header *yeti.h*:
~~~~~~
#define DISABLE_LOGGING
// ...
#include <yeti/yeti.h>
~~~~~~
This instruction sets all logging macros to <i>((void)0)</i>.


### List of Control Functions ###

Logging control is based on *yeti* namespace functions:
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
  void FlushLog();
  void RegAllSignals(__sighandler_t signal_handler = SimpleSignalHandler);
  void RegSignal(int sig_num, __sighandler_t signal_handler = SimpleSignalHandler);
}
~~~~~~

To get more information about all this functions build documentation using doxygen
and search their descriptions there.


## Requirements ##

* clang v3.4 or later, gcc v4.8 or later;
* pthread
* scons v2.3.3 or later

**Yeti** uses C++11 and multithreading, so you should add *-std=c++11*
and *-pthread* compile and link options.

**Yeti** uses SCons as a build system.
So to build project and run *output_test* just type in your console:

~~~~~~
# scons
# LD_LIBRARY_PATH="./build" ./build/tests/output_test
~~~~~~

## Example

**Example to test Yeti output:**
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
  yeti::RegAllSignals();

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

**OUTPUT: stderr**
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

**OUTPUT: /tmp/test.log**
~~~~~~
21 [WARNING] <3642E50D01F30105> tests/output_test.cc: 39: current file descriptor: 3
22 [ERROR] <3642E50D01F30105> tests/output_test.cc: 42: print array elements: (0, 1, 2)
23 [CRITICAL] <3642E50D01F30105> tests/output_test.cc: 44: current log level: 2

[ERROR] [12446:3642E50D01F30105] TestLog(): print array elements: (0, 1, 2)
[CRITICAL] [12446:3642E50D01F30105] TestLog(): current log level: 1

[CRITICAL] [12446:3642E50D01F30105] TestLog(): current log level: 0
~~~~~~

## Licence ##

**Yeti** is distributed with a BSD license


## Documentation ##

Use doxygen to create documentation:
~~~~~~
cd <yeti-root>/docs  # where <yeti-root> -- path to yeti project
                     # (example: /home/user/workspace/yeti)
doxygen Doxyfile
~~~~~~
