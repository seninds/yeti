C++ logging system
==================

__Yeti__ is lightweight threadsafe logging system, which is running in separate thread,
so it can't slow your application. 

It has 6 logging levels:
```cpp
enum LogLevel {
  LOG_LEVEL_CRITICAL,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_TRACE
}
```

Logging has printf-style compact format:
```cpp
ERROR("[%d] exception: %s", function_id. e.what());
```

You can tune log format:
```cpp
yeti::SetFormatStr("[%(TAG)] [%(PID)] %(FILENAME): %(LINE): %(MSG)");
```
Keywords to set log format are:
* %(TAG)      - tag of current logging level
* %(FILENAME) - filename
* %(FUNCNAME) - function name
* %(PID)      - process ID
* %(TID)      - thread ID
* %(LINE)     - line number
* %(MSG)      - user message (format string)

## Usage

Just add content of _"inc/yeti"_ to your project, add __yeti.h__
into your source files and start to use __Yeti__ log.

Logging system has a lazy initialization, so if you don't use it you will not have any overhead.
Just add into your code following macros:
```cpp
CRITICAL(msg_fmt, ...);
ERROR(msg_fmt, ...);
WARN(msg_fmt, ...);
INFO(msg_fmt, ...);
DEBUG(msg_fmt, ...);
TRACE(msg_fmt, ...);
```

Logging control is based on yeti namespace functions:
```cpp
namespace yeti {
  void SetLevel(LogLevel level) noexcept;
  int GetLevel() noexcept;
  void SetColored(bool is_colored) noexcept;
  bool GetColored() noexcept;
  void SetFileDesc(FILE* fd) noexcept;
  FILE* GetFileDesc() noexcept;
  void CloseFile(FILE* fd = nullptr);
  void SetFormatStr(const std::string& format_str) noexcept;
  std::string GetFormatStr() noexcept;
}
```

## Requirements

__Yeti__ uses C++11 and multithreading, so you should add "-std=c++11 -pthread" compile options:

```shell
g++ -O3 -std=c++11 -Wall -Werror -pthread -I../inc -o output_test output_test.cc
```


## Example

```cpp
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
```

## LICENCE

__Yeti__ is distributed with a BSD license
