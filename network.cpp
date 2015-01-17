#include "network.h"
#include <boost/property_tree/json_parser.hpp>

namespace shell {
  void exec(const std::string&);
}

using std::string;

void downloadWeatherData(const std::string& location)
{
  std::string shellCommmand = "wget http://api.openweathermap.org/data/2.5/weather?q=" + location + " -O " + location + " -o logFile_"+location;
  shell::exec(shellCommmand);
}
