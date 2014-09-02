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
#include <iostream>
#include <yeti/yeti.h>


void TestLog(yeti::LogLevel level) {
  yeti::SetLevel(level);
  CRITICAL("critical output: %d", 0);
  ERROR("error output: %f", 2.5F);
  WARN("warning output: %s", "test text");
  INFO("info output sizeof(int) = %zu", sizeof(int));
  DEBUG("debug output %s", "test debug output");
  TRACE("trace info: function trace");
  CRITICAL("\n");
}


int main(int argc, char* argv[]) {
  TestLog(yeti::LOG_LEVEL_TRACE);
  TestLog(yeti::LOG_LEVEL_DEBUG);
  TestLog(yeti::LOG_LEVEL_INFO);
  TestLog(yeti::LOG_LEVEL_WARNING);
  TestLog(yeti::LOG_LEVEL_ERROR);
  TestLog(yeti::LOG_LEVEL_CRITICAL);

  return 0;
}
```

## LICENCE

__Yeti__ is distributed with a BSD license
