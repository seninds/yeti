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

#ifndef INC_YETI_MACRO_H_
#define INC_YETI_MACRO_H_

#define CRITICAL(fmt, ...) { \
  std::string func_name(__func__); \
  bool is_colored = yeti::Logger::instance().GetColored(); \
  FILE *fd = yeti::Logger::instance().GetFileDesc(); \
  auto print_func = [func_name, is_colored, fd]() { \
    if (isatty(fileno(fd)) != 0 && is_colored) { \
        std::fprintf(fd, YETI_LRED "%-10s %s: %d: %s(): " YETI_RESET fmt \
                     "\n", "[CRITICAL]", __FILE__, __LINE__, func_name.c_str(), \
                     ##__VA_ARGS__); \
      } else { \
        std::fprintf(fd, "%-10s %s: %d: %s(): " fmt "\n", "[CRITICAL]", \
                     __FILE__, __LINE__, func_name.c_str(), ##__VA_ARGS__); \
      } \
  }; \
  yeti::Logger::instance().PushToQueue(print_func); \
}

#define ERROR(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_ERROR) { \
    std::string func_name(__func__); \
    bool is_colored = yeti::Logger::instance().GetColored(); \
    FILE *fd = yeti::Logger::instance().GetFileDesc(); \
    auto print_func = [func_name, is_colored, fd]() { \
      if (isatty(fileno(fd)) != 0 && is_colored) { \
        std::fprintf(fd, YETI_LPURPLE "%-10s %s: %d: %s(): " YETI_RESET fmt \
                     "\n", "[ERROR]", __FILE__, __LINE__, func_name.c_str(), \
                     ##__VA_ARGS__); \
      } else { \
        std::fprintf(fd, "%-10s %s: %d: %s(): " fmt "\n", "[ERROR]", \
                     __FILE__, __LINE__, func_name.c_str(), ##__VA_ARGS__); \
      } \
    }; \
    yeti::Logger::instance().PushToQueue(print_func); \
  } \
}

#define WARN(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_WARNING) { \
    std::string func_name(__func__); \
    bool is_colored = yeti::Logger::instance().GetColored(); \
    FILE *fd = yeti::Logger::instance().GetFileDesc(); \
    auto print_func = [func_name, is_colored, fd]() { \
      if (isatty(fileno(fd)) != 0 && is_colored) { \
        std::fprintf(fd, YETI_YELLOW "%-10s %s: %d: %s(): " YETI_RESET fmt \
                     "\n", "[WARNING]", __FILE__, __LINE__, func_name.c_str(), \
                     ##__VA_ARGS__); \
      } else { \
        std::fprintf(fd, "%-10s %s: %d: %s(): " fmt "\n", "[WARNING]", \
                     __FILE__, __LINE__, func_name.c_str(), ##__VA_ARGS__); \
      } \
    }; \
    yeti::Logger::instance().PushToQueue(print_func); \
  } \
}

#define INFO(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_INFO) { \
    std::string func_name(__func__); \
    bool is_colored = yeti::Logger::instance().GetColored(); \
    FILE *fd = yeti::Logger::instance().GetFileDesc(); \
    auto print_func = [func_name, is_colored, fd]() { \
      if (isatty(fileno(fd)) != 0 && is_colored) { \
        std::fprintf(fd, YETI_LGREEN "%-10s %s: %d: %s(): " YETI_RESET fmt \
                     "\n", "[INFO]", __FILE__, __LINE__, func_name.c_str(), \
                     ##__VA_ARGS__); \
      } else { \
        std::fprintf(fd, "%-10s %s: %d: %s(): " fmt "\n", "[INFO]", \
                     __FILE__, __LINE__, func_name.c_str(), ##__VA_ARGS__); \
      } \
    }; \
    yeti::Logger::instance().PushToQueue(print_func); \
  } \
}

#define DEBUG(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_DEBUG) { \
    std::string func_name(__func__); \
    bool is_colored = yeti::Logger::instance().GetColored(); \
    FILE *fd = yeti::Logger::instance().GetFileDesc(); \
    auto print_func = [func_name, is_colored, fd]() { \
      if (isatty(fileno(fd)) != 0 && is_colored) { \
        std::fprintf(fd, YETI_WHITE "%-10s %s: %d: %s(): " YETI_RESET fmt \
                     "\n", "[DEBUG]", __FILE__, __LINE__, func_name.c_str(), \
                     ##__VA_ARGS__); \
      } else { \
        std::fprintf(fd, "%-10s %s: %d: %s(): " fmt "\n", "[DEBUG]", \
                     __FILE__, __LINE__, func_name.c_str(), ##__VA_ARGS__); \
      } \
    }; \
    yeti::Logger::instance().PushToQueue(print_func); \
  } \
}

#define TRACE(fmt, ...) { \
  if (yeti::Logger::instance().GetLevel() >= yeti::LOG_LEVEL_TRACE) { \
    std::string func_name(__func__); \
    FILE *fd = yeti::Logger::instance().GetFileDesc(); \
    auto print_func = [func_name, fd]() { \
      std::fprintf(fd, "%-10s %s: %d: %s(): " fmt "\n", "[TRACE]", \
                   __FILE__, __LINE__, func_name.c_str(), ##__VA_ARGS__); \
    }; \
    yeti::Logger::instance().PushToQueue(print_func); \
  } \
}

#endif  // INC_YETI_MACRO_H_
