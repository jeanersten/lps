#pragma once

#include <format>
#include <iostream>
#include <string_view>

#if defined(_MSC_VER)

#include <intrin.h>

#define LPS_DEBUG_BREAK() __debugbreak()

#else

#define LPS_DEBUG_BREAK()

#endif

namespace LPS
{
  enum class LogLevel
  {
    Trace,
    Info,
    Warn,
    Error,
    Fatal
  };

  class Log
  {
  public:
    template<typename... Args>
    static inline void Print(LogLevel level,
                             std::format_string<Args...> str_fmt,
                             Args&&... args)
    {
      if (level >= s_level)
      {
        auto str_fmted{ std::format(str_fmt, std::forward<Args>(args)...) };

        std::cout << std::format("{}{}{} {}{:6}{}: {}\n",
                                 TEXT_COLOR_GRAY,
                                 GetCurrentTimeString(),
                                 TEXT_COLOR_RESET,
                                 GetLevelColorString(level),
                                 GetLevelString(level),
                                 TEXT_COLOR_RESET,
                                 str_fmted);
      }
    }

    static inline void SetLevel(LogLevel level)
    {
      s_level = level;
    }

  private:
    Log() = delete;
    ~Log() = delete;

    static constexpr std::string_view TEXT_TRACE{ "TRACE" };
    static constexpr std::string_view TEXT_INFO { "INFO" };
    static constexpr std::string_view TEXT_WARN { "WARN" };
    static constexpr std::string_view TEXT_ERROR{ "ERROR" };
    static constexpr std::string_view TEXT_FATAL{ "FATAL" };

    static constexpr std::string_view TEXT_COLOR_RESET  { "\033[0m" };
    static constexpr std::string_view TEXT_COLOR_GRAY   { "\x1B[90m" };
    static constexpr std::string_view TEXT_COLOR_RED    { "\033[91m" };
    static constexpr std::string_view TEXT_COLOR_GREEN  { "\033[92m" };
    static constexpr std::string_view TEXT_COLOR_YELLOW { "\033[93m" };
    static constexpr std::string_view TEXT_COLOR_BLUE   { "\033[94m" };
    static constexpr std::string_view TEXT_COLOR_MAGENTA{ "\033[95m" };

    static LogLevel s_level;

    static std::string_view GetLevelString(LogLevel level);
    static std::string_view GetLevelColorString(LogLevel level);
    static std::string_view GetCurrentTimeString();
  };
}

#if defined(LPS_DEBUG)


#define LPS_SET_LOG_LEVEL(LVL) LPS::Log::SetLevel(LVL)
#define LPS_LOG_TRACE(...) \
LPS::Log::Print(LPS::LogLevel::Trace, ##__VA_ARGS__)
#define LPS_LOG_INFO(...)  \
LPS::Log::Print(LPS::LogLevel::Info, ##__VA_ARGS__)
#define LPS_LOG_WARN(...)  \
LPS::Log::Print(LPS::LogLevel::Warn, ##__VA_ARGS__)
#define LPS_LOG_ERROR(...) \
LPS::Log::Print(LPS::LogLevel::Error, ##__VA_ARGS__)
#define LPS_LOG_FATAL(...) \
LPS::Log::Print(LPS::LogLevel::Fatal, ##__VA_ARGS__)

#define LPS_ASSERT(EXPR, MSG)                           \
do                                                      \
{                                                       \
  if (!(EXPR))                                          \
  {                                                     \
    LPS_LOG_FATAL("Assertion '{}' failed at {}:{}, {}", \
                  #EXPR, __FILE__, __LINE__, MSG);      \
    LPS_DEBUG_BREAK();                                  \
  }                                                     \
}                                                       \
while (false)

#else

#define LPS_SET_LOG_LEVEL(LVL)
#define LPS_LOG_TRACE(...)
#define LPS_LOG_INFO(...)
#define LPS_LOG_WARN(...)
#define LPS_LOG_ERROR(...)
#define LPS_LOG_FATAL(...)

#define LPS_ASSERT(EXPR, MSG)

#endif
