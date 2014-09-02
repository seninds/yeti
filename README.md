C++ logging system
==================

__Yeti__ is lightweight threadsafe logging system, which is running in separate thread.
So it can't slow your application.

Logging has printf-style compact format:
```cpp
ERROR("[%d] exception: %s", function_id. e.what());
```

## Usage

Just add __yeti.h__ into your project and start to use it:
```cpp
#include <yeti/yeti.h>
```
Logging system has a lazy initialization, so if you don't use it you will not have any overhead.

## Requirements

__Yeti__ uses C++11 and multithreading, so you should add "-std=c++11 -pthread" compile options:

```shell
g++ -O3 -std=c++11 -Wall -Werror -pthread -I../inc -o output_test output_test.cc
```


## Example

```cpp
#include <yeti/yeti.h>

#ifdef _WIN32
#  define TMP_DIR "C:\\Temp"
#else
#  define TMP_DIR "/tmp"
#endif  // _WIN32


void TestLog(yeti::LogLevel level) {
  yeti::SetLevel(level);  // set current log level

  TRACE("trace info: function trace");
  DEBUG("debug output %s", "test debug output");
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
  TestLog(yeti::LOG_LEVEL_INFO);

  FILE* fd = ::fopen(TMP_DIR "/output_test.log", "w");
  yeti::SetFileDesc(fd);  // start logging into specified file
  TestLog(yeti::LOG_LEVEL_WARNING);
  TestLog(yeti::LOG_LEVEL_ERROR);
  yeti::SetColored(true);
  TestLog(yeti::LOG_LEVEL_CRITICAL);  // check is a tty device currently using
  yeti::CloseFile();

  return 0;
}
```

## LICENCE

__Yeti__ is distributed with a BSD license
