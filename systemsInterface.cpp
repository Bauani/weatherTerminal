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

  vector<string> ret;

  // Present all arguments:
  for (int iter = 1; iter<argCount; ++iter) {
    cout<<GREENCOLOR <<argValues[iter] <<DEFAULTCOLOR <<'\n';

    // when argument starts with a dash, it specifies what to sort for.
    //  save this as the first argument
    if (argValues[iter][0] == '-') {
      static bool searchOptionAllreadyGiven = false;
      if (searchOptionAllreadyGiven) {
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
          searchOptionAllreadyGiven = true;
          continue;
        case 'l':
          cout<<"Sort by location\n";
          WeatherData::setComparisonFunction( 
              [](WeatherData const& lhs, WeatherData const& rhs) -> bool 
          { return lhs.name() < rhs.name(); });
          ++iter;
          searchOptionAllreadyGiven = true;
          continue;
        case 'p':
          cout<<"Sort by pressure\n";
          WeatherData::setComparisonFunction( 
              [](WeatherData const& lhs, WeatherData const& rhs) -> bool 
          { return lhs.pressure() < rhs.pressure(); });
          ++iter;
          searchOptionAllreadyGiven = true;
          continue;
        case 'h':
          cout<<"Sort by humidity\n";
          WeatherData::setComparisonFunction( 
              [](WeatherData const& lhs, WeatherData const& rhs) -> bool 
          { return lhs.humidity() < rhs.humidity(); });
          ++iter;
          searchOptionAllreadyGiven = true;
          continue;
        default:
          cout<<REDCOLOR <<"warning" <<DEFAULTCOLOR <<": unknown option [" <<argValues[iter] <<"]\n";
          continue;
      }
    }

    // Not 'option' - read place directly into array
    ret.push_back(argValues[iter]);
  }
  return ret;
}

void shell::writeHelpText(const string& softwareCall)
{
  //cout<<GREYTEXT <<"Use:" <<DEFAULTCOLOR
  cout
      <<softwareCall <<" [options] [places]\n"
      <<"\t[options]: \t -s[element to sort by] default: name\n"
      <<"\t[places]:  \t places to find weather for.";
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


