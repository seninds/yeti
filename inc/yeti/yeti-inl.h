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

#ifndef INC_YETI_YETI_INL_H_
#define INC_YETI_YETI_INL_H_

#include <cstdio>
#include <yeti/logger.h>

namespace yeti {

inline void SetLogLevel(LogLevel level) noexcept {
  Logger::instance().SetLevel(level);
}

inline int GetLogLevel() noexcept {
  return Logger::instance().GetLevel();
}

inline void SetLogColored(bool is_colored) noexcept {
  Logger::instance().SetColored(is_colored);
}

inline bool IsLogColored() noexcept {
  return Logger::instance().IsColored();
}

inline void SetLogFileDesc(FILE* fd) noexcept {
  Logger::instance().SetFileDesc(fd);
}

inline FILE* GetLogFileDesc() noexcept {
  return Logger::instance().GetFileDesc();
}

inline void CloseLogFileDesc(FILE* fd) {
  Logger::instance().CloseFileDesc(fd);
}

inline void SetLogFormatStr(const std::string& format_str) noexcept {
  Logger::instance().SetFormatStr(format_str);
}

inline std::string GetLogFormatStr() noexcept {
  return Logger::instance().GetFormatStr();
}

}  // namespace yeti

#endif  // INC_YETI_YETI_INL_H_
