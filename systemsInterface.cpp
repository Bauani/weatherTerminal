#include "systemsInterface.h"
#include "weatherData.h"
#include "colors.h"

using std::vector;
using std::string;
using std::cout;

vector<string> shell::readArgument(int argCount, char *argValues[])
{
  if (argCount < 2) {
    cout<<"Can not find weather unless user specifies place to look for.\n";
    writeHelpText(argValues[0]);
  }

  // If no search option is supplied, print help text (at end of function)
  bool searchOptionSupplied = false;

  vector<string> ret;

  // Present all arguments:
  for (int iter = 1; iter<argCount; ++iter) {
 #ifdef DEBUG
    cout<<GREENCOLOR <<argValues[iter] <<DEFAULTCOLOR <<'\n';
#endif

    // when argument starts with a dash, it specifies what to sort for.
    //  save this as the first argument
    if (argValues[iter][0] == '-') {
      if (searchOptionSupplied) {
        cout<<REDCOLOR <<"warning" <<DEFAULTCOLOR 
            <<": search option allready configured. Ignores option " 
            <<GREYTEXT <<argValues[iter] <<DEFAULTCOLOR <<'\n';
            break;
      }

      switch (argValues[iter][1]) {
        case 't':
          cout<<"Sort by temperature\n";
          WeatherData::setComparisonFunction( 
              [](WeatherData const& lhs, WeatherData const& rhs) -> bool 
            { return lhs.temperature() < rhs.temperature(); });
          ++iter;
          searchOptionSupplied = true;
          continue;
        case 'l':
          cout<<"Sort by location\n";
          WeatherData::setComparisonFunction( 
              [](WeatherData const& lhs, WeatherData const& rhs) -> bool 
          { return lhs.name() < rhs.name(); });
          ++iter;
          searchOptionSupplied = true;
          continue;
        case 'p':
          cout<<"Sort by pressure\n";
          WeatherData::setComparisonFunction( 
              [](WeatherData const& lhs, WeatherData const& rhs) -> bool 
          { return lhs.pressure() < rhs.pressure(); });
          ++iter;
          searchOptionSupplied = true;
          continue;
        case 'h':
          cout<<"Sort by humidity\n";
          WeatherData::setComparisonFunction( 
              [](WeatherData const& lhs, WeatherData const& rhs) -> bool 
          { return lhs.humidity() < rhs.humidity(); });
          ++iter;
          searchOptionSupplied = true;
          continue;
        default:
          cout<<REDCOLOR <<"warning" <<DEFAULTCOLOR <<": unknown option [" <<argValues[iter] <<"]\n";
          continue;
      }
    }

    // Not 'option' - read place directly into array
    ret.push_back(argValues[iter]);
  }
  if (!searchOptionSupplied)
    shell::writeHelpText(argValues[0]);

  return ret;
}

void shell::writeHelpText(const string& softwareCall)
{
  // small guard: help text should only be printed once during each execution.
  static bool textAllreadyPrinted = false;
  if (textAllreadyPrinted)
    return;
  textAllreadyPrinted = true;

  cout<<YELLOWCOLOR <<"Use:" <<DEFAULTCOLOR 
      <<GREYTEXT <<"\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" <<DEFAULTCOLOR
      <<softwareCall <<"\t[search option] [places]\n"
      <<"\t[options]: \t\tDefault: sort by name\n"
      <<"\t\t\t -t\tsort elements by temperature\n"
      <<"\t\t\t -p\tsort elements by pressure\n"
      <<"\t\t\t -h\tsort elements by humidity\n"
      <<"\t\t\t -l\tsort elements by location (default)\n"
      <<"\t[places]:  \t places to find weather for.\n"
      <<GREYTEXT <<"*  *  *\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" <<DEFAULTCOLOR;
} 

int shell::exec(const string& cmd)
{
  int returnFromSystemCall = std::system(cmd.c_str());

  if (returnFromSystemCall) {
    cout<<REDCOLOR <<"warning: " <<DEFAULTCOLOR <<"Could not execute system call with shell::exec(\"" <<cmd <<"\")\n";
    return -1;
  }

  return 0;
}


