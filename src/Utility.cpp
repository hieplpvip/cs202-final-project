#include "Utility.h"

bool fileExists(const std::string& name) {
  std::ifstream f(name.c_str());
  return f.good();
}
