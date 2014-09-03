/// @mainpage Yeti Project
///
/// C++ logging system
/// ==================
///
/// <b>Yeti</b> is lightweight threadsafe logging system, which is running in
/// separate thread, so it can't slow your application.
///
/// It has 6 logging levels:
/// @code{.cpp}
/// enum LogLevel {
///   LOG_LEVEL_CRITICAL,
///   LOG_LEVEL_ERROR,
///   LOG_LEVEL_WARNING,
///   LOG_LEVEL_INFO,
///   LOG_LEVEL_DEBUG,
///   LOG_LEVEL_TRACE
/// }
/// @endcode
///
/// Logging has printf-style compact format:
/// @code{.cpp}
/// ERROR("[%d] exception: %s", function_id. e.what());
/// @endcode
///
/// You can tune log format:
/// @code{.cpp}
/// yeti::SetFormatStr("[%(TAG)] [%(PID)] %(FILENAME): %(LINE): %(MSG)");
/// @endcode
///
/// Keywords to set log format are:
/// <ul>
/// <li> %(TAG)      - tag of current logging level </li>
/// <li> %(FILENAME) - filename </li>
/// <li> %(FUNCNAME) - function name </li>
/// <li> %(PID)      - process ID </li>
/// <li> %(TID)      - thread ID </li>
/// <li> %(LINE)     - line number </li>
/// <li> %(MSG)      - user message (format string) </li>
/// </ul>
///
/// @section Usage
///
/// Just add <b>yeti.h</b> into your project and start to use it:
/// @code{.cpp}
/// #include <yeti/yeti.h>
/// @endcode
///
/// Logging system has a lazy initialization, so if you don't use it you will
/// not have any overhead. Just add into your code following macros:
/// @code{.cpp}
/// CRITICAL(msg_fmt, ...);
/// ERROR(msg_fmt, ...);
/// WARN(msg_fmt, ...);
/// INFO(msg_fmt, ...);
/// DEBUG(msg_fmt, ...);
/// TRACE(msg_fmt, ...);
/// @endcode
///
/// Logging control is based on yeti namespace functions:
/// @code{.cpp}
/// void SetLevel(LogLevel level) noexcept;
/// int GetLevel() noexcept;
/// void SetColored(bool is_colored) noexcept;
/// bool GetColored() noexcept;
/// void SetFileDesc(FILE* fd) noexcept;
/// FILE* GetFileDesc() noexcept;
/// void CloseFile(FILE* fd = nullptr);
/// void SetFormatStr(const std::string& format_str) noexcept;
/// std::string GetFormatStr() noexcept;
/// @endcode
///
/// More information you can get <a href="namespaceyeti.html">here</a>.
///
/// @section Requirements
///
/// <b>Yeti</b> uses C++11 and multithreading, so you should add
/// "-std=c++11 -pthread" compile options:
/// @code
/// g++ -O3 -std=c++11 -Wall -Werror -pthread -I../inc -o output_test output_test.cc
/// @endcode
///
/// @section Example
///
/// @code{.cpp}
/// #include <yeti/yeti.h>
///
/// void TestLog(yeti::LogLevel level) {
///   yeti::SetLevel(level);  // set current log level
///   TRACE("trace info: function trace");
///   DEBUG("debug output %s", "test debug output");
///   DEBUG("debug simple output");
///   INFO("info output sizeof(int) = %zu", sizeof(int));
///   WARN("warning output: %s", "test text");
///   ERROR("error output: %f", 2.5F);
///   CRITICAL("critical output: %d\n", 0);
/// }
///
///
/// int main(int argc, char* argv[]) {
///   yeti::SetColored(true);  // turn on log colorization
///   TestLog(yeti::LOG_LEVEL_TRACE);
///   TestLog(yeti::LOG_LEVEL_DEBUG);
///
///   yeti::SetColored(false);  // turn off log colorization
///   yeti::SetFormatStr("[%(TAG)] [%(PID)] %(FILENAME): %(LINE): %(MSG)");
///   TestLog(yeti::LOG_LEVEL_INFO);
///
///   FILE* fd = ::fopen("output_test.log", "w");
///   yeti::SetFileDesc(fd);  // start logging into specified file
///   TestLog(yeti::LOG_LEVEL_WARNING);
///   yeti::SetFormatStr("[%(TAG)] [%(PID):%(TID)] %(FUNCNAME)(): %(MSG)");
///   TestLog(yeti::LOG_LEVEL_ERROR);
///   yeti::SetColored(true);
///   TestLog(yeti::LOG_LEVEL_CRITICAL);  // check is a tty device currently using
///   yeti::CloseFile();
///
///   return 0;
/// }
/// @endcode
///
/// @section LICENCE
///
/// <b>Yeti</b> is distributed with a BSD license
