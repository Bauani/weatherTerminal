#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "weatherData.h"

namespace shell {
  std::vector<std::string> readArgument(int argc, char *argv[]);
    /* Function readInputArgumets() - read shell input arguments
    *   Ret.: List of strings where the first element gives what 
    *   to sort output by, and the rest gives the locations to
    *   search for.
    */

  void writeHelpText(const std::string&);

  int exec(const std::string&);

}

#endif
