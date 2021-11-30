#ifndef LOGGING_H
#define LOGGING_H

namespace Logging {
  extern void setLevel(int level);
  extern void info(const char* fmt, ...);
  extern void debug(const char* fmt, ...);
  extern void error(const char* fmt, ...);
};

#endif
