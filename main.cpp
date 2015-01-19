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


  // Get weather data. Download and parse data from each location in separate threads.
  vector<shared_ptr<thread> > allThreads;

  vector<shared_ptr<WeatherData>> allLocationsData;

  // Make threads. Get data and parse JSON to fill WeatherData objects
  for (string& locationIter : shellArguments) {
    // Without threads:
    //   allLocationsData.push_back(shared_ptr<WeatherData>(new WeatherData(locationIter)));
    // With threads:
    allLocationsData.push_back(shared_ptr<WeatherData>(std::make_shared<WeatherData>()));
    allThreads.push_back( std::make_shared<thread>(&WeatherData::load, allLocationsData.back(), locationIter));
  }

  // Wait for all threads to finish
  for (auto threadIter : allThreads)
    threadIter->join();

  // Sort by name, so that all empty objects are at the end. Then remove tail.:
  std::sort( allLocationsData.begin(), allLocationsData.end(), 
      [](const shared_ptr<WeatherData>& pL, const shared_ptr<WeatherData>& pR){ return (pL->name()>pR->name()); });
  // Remove all empty objects (where the supplied location could not be found)
  std::vector<shared_ptr<WeatherData> >::iterator firstEmptyElement = allLocationsData.begin();
  for (auto wdIter : allLocationsData) {
    if (!wdIter->excists()) {
      cout<<REDCOLOR <<"warninig" <<DEFAULTCOLOR <<"Could not find all supplied locations. Please check spelling\n";
      break; 
    }
    ++firstEmptyElement;
  }
  allLocationsData.erase(firstEmptyElement, allLocationsData.end());

  // Get custom sort algorith from static member in WeatherData (initialized 
  //   from systemsInterface::readArgument(argc,**argv)
  std::function<bool (const WeatherData& lhs, const WeatherData& rhs)> greater = WeatherData::comparisonFunction();
  std::sort(  allLocationsData.begin(),
              allLocationsData.end(),
              [&greater](const shared_ptr<WeatherData>& pL, const shared_ptr<WeatherData>& pR){ return greater(*pL, *pR); });

  for (auto& iter : allLocationsData)
    //cout<<iter->name() <<"\tTemp:" <<iter->temperature()-273.15 <<"\tPressure:" <<iter->pressure() <<"\n";
    iter->printData();

  cout<<'\n';

}
