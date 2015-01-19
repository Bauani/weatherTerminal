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


  cout<<REDCOLOR <<"Try with multiple different threads\n";

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
  cout<<REDCOLOR <<"all threads completed\n\n"<<DEFAULTCOLOR;

  // Sort by name, so that all empty objects are at the end. Then remove tail.:
  std::sort( allLocationsData.begin(), allLocationsData.end(), 
      [](const shared_ptr<WeatherData>& pL, const shared_ptr<WeatherData>& pR){ return (pL->name()>pR->name()); });

  // Remove all empty objects (where the supplied location could not be found)
  //   (at this point, list is sorted by name and all empty names is at the end)
  std::vector<shared_ptr<WeatherData> >::iterator firstEmptyElement = allLocationsData.begin();
  for (auto wdIter : allLocationsData) {
    if (!wdIter->excists()) {
      cout<<"Found non-excisting iter obj\tDeletes all entries that could not be found\n";
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
    cout<<iter->name() <<"\tTemp:" <<iter->temperature()-273.15 <<"\tPressure:" <<iter->pressure() <<"\n";

  cout<<'\n';
  

  // 1) Sjekk om første argument er en Option (for sortering). Dette kan 
  //   gjøres ved å samanlikne med predefinerte sorteringsalgoritme-strengar
  //   som skrives først dersom rett options sendast inn.
  //     I så fall: ta hånd om dette og iterer til neste

  // 2) iterer gjennom alle stadane og start tråd for nedlasting av værdata. **Denne kan kanskje også gjøre (pkt. (3)??)**

  // 3) bruk boost::property_tree for å hente ut verdiar.

  //TEST:
  string testLocation("Oslo");
  WeatherData osloTest(testLocation);
  if (osloTest.excists()) {
    osloTest.printData();
  } else {
    cout<<"Node " <<REDCOLOR <<testLocation <<DEFAULTCOLOR <<" does not excist\n";
  }
  cout<<"Finalizing\n";
}

/* Plan: Make a separate function that gets weather data and parses JSON file (fill WeatherData objects). 
 *   - This function is to be used in the threads
 *   - Function takes argument: The list that is to be filled with WeatherData pointers (heap objects)
 *
 */

