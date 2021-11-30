#ifdef _WIN32
#include <Windows.h>
#endif
#include <cstdio>

namespace Logging {
  enum class TextColor {
    WHITE = 1,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
  };

  void setTextColor(TextColor c) {
#ifdef _WIN32
    switch (c) {
      case TextColor::WHITE:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        break;
      case TextColor::RED:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        break;
      case TextColor::GREEN:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        break;
      case TextColor::YELLOW:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        break;
      case TextColor::BLUE:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
        break;
      case TextColor::MAGENTA:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
        break;
      case TextColor::CYAN:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        break;
    }
#else
    switch (c) {
      case TextColor::WHITE:
        std::cout << "\u001b[37m";
        break;
      case TextColor::RED:
        std::cout << "\u001b[31m";
        break;
      case TextColor::GREEN:
        std::cout << "\u001b[32m";
        break;
      case TextColor::YELLOW:
        std::cout << "\u001b[33m";
        break;
      case TextColor::BLUE:
        std::cout << "\u001b[34m";
        break;
      case TextColor::MAGENTA:
        std::cout << "\u001b[35m";
        break;
      case TextColor::CYAN:
        std::cout << "\u001b[36m";
        break;
    }
#endif
  }

  // 0: only info
  // 1: info + error
  // 2: info + error + debug
  int logging_level = 0;

  void setLevel(int level) {
    logging_level = level;
  }

  void info(const char* fmt, ...) {
    setTextColor(TextColor::GREEN);
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
    setTextColor(TextColor::WHITE);
  }

  void debug(const char* fmt, ...) {
    if (logging_level < 2) return;
    setTextColor(TextColor::YELLOW);
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
    setTextColor(TextColor::WHITE);
  }

  void error(const char* fmt, ...) {
    if (logging_level < 1) return;
    setTextColor(TextColor::RED);
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
    setTextColor(TextColor::WHITE);
  }
};
