#include <iostream>
#include <string>
#include <thread>

#include "systemsInterface.h"
#include "network.h"
#include "colors.h"
#include "weatherData.h"

#include <iostream>
#include <vector>

#include <memory>

using std::vector;
using std::string;
using std::cout;
using std::shared_ptr;
using std::thread;


int main(int argc, char *argv[])
{
  vector<string> shellArguments = shell::readArgument(argc, argv); 



  cout<<GREYTEXT <<"Shell arguments:\n" <<DEFAULTCOLOR;
  for (auto iter : shellArguments)
    cout<<"Arg.: " <<YELLOWCOLOR <<iter <<DEFAULTCOLOR <<"\n";

  cout<<"\n\n";




  vector<shared_ptr<WeatherData>> allLocationsData;
  /*
   * TODO Plan: Make a function that decides which param to sort by. 
   *    This funciton should pop the first element of the vector if this is an option argument.
   *    Othervise: NOP
   *  (this is important for next loop to work right)
   */
  for (string& iter : shellArguments) {
    allLocationsData.push_back(shared_ptr<WeatherData>(new WeatherData(iter)));
    // TODO Check whether the location is registered (if name : 
  }

  // Get custom sort algorith from static member in WeatherData (initialized 
  //   from systemsInterface::readArgument(argc,**argv)
  std::function<bool (const WeatherData& lhs, const WeatherData& rhs)> greater = WeatherData::comparisonFunction();
  std::sort(  allLocationsData.begin(),
              allLocationsData.end(),
              [&greater](const shared_ptr<WeatherData>& pL, const shared_ptr<WeatherData>& pR){ return greater(*pL, *pR); });

  for (auto& iter : allLocationsData)
    cout<<iter->name() <<"\tTemp:" <<iter->temperature() <<"\tPressure:" <<iter->pressure() <<"\n";

  cout<<'\n';
  

  // 1) Sjekk om første argument er en Option (for sortering). Dette kan 
  //   gjøres ved å samanlikne med predefinerte sorteringsalgoritme-strengar
  //   som skrives først dersom rett options sendast inn.
  //     I så fall: ta hånd om dette og iterer til neste

  // 2) iterer gjennom alle stadane og start tråd for nedlasting av værdata. **Denne kan kanskje også gjøre (pkt. (3)??)**

  // 3) bruk boost::property_tree for å hente ut verdiar.

  WeatherData osloTest("Oslo");
  if (osloTest.excists()) {
    osloTest.printData();
  } else {
    cout<<"Node does not excist\n";
  }
}


