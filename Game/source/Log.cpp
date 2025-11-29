#include "LPSPCH.hpp"
#include "Log.hpp"

namespace LPS
{
  LogLevel Log::s_level{ LogLevel::Trace };

  std::string_view Log::GetLevelString(LogLevel level)
  {
    switch(level)
    {
      case LogLevel::Trace:
      {
        return TEXT_TRACE;
      }
      break;

      case LogLevel::Info:
      {
        return TEXT_INFO;
      }
      break;

      case LogLevel::Warn:
      {
        return TEXT_WARN;
      }
      break;

      case LogLevel::Error:
      {
        return TEXT_ERROR;
      }

      case LogLevel::Fatal:
      {
        return TEXT_FATAL;
      }

      default:
      {
        return "";
      }
      break;
    }
  }

  std::string_view Log::GetLevelColorString(LogLevel level)
  {
    switch(level)
    {
      case LogLevel::Trace:
      {
        return TEXT_COLOR_BLUE;
      }
      break;

      case LogLevel::Info:
      {
        return TEXT_COLOR_GREEN;
      }
      break;

      case LogLevel::Warn:
      {
        return TEXT_COLOR_YELLOW;
      }
      break;

      case LogLevel::Error:
      {
        return TEXT_COLOR_RED;
      }
      break;

      case LogLevel::Fatal:
      {
        return TEXT_COLOR_MAGENTA;
      }
      break;

      default:
      {
        return "";
      }
      break;
    }
  }

  std::string_view Log::GetCurrentTimeString()
  {
    static std::string time_str{ };
    std::chrono::time_point current_tp{ std::chrono::floor<std::chrono::seconds>
                                        (std::chrono::system_clock::now()) };

    time_str = std::format("{:%d-%m-%Y %H:%M:%S}",
                           std::chrono::current_zone()->to_local(current_tp));

    return time_str;
  }
}
