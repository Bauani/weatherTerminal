#include "weatherData.h"
#include "network.h"
#include "colors.h"

#include <iostream>
#include <functional>

using std::cout;
using std::string;

std::function<bool (const WeatherData&, const WeatherData&)> WeatherData::priorTo = [](const WeatherData& lhs, const WeatherData& rhs) -> bool{ return lhs.name()>rhs.name(); };

WeatherData::WeatherData(string location)
{
  load(location);
}

void WeatherData::load(std::string location)
{
  using boost::property_tree::ptree;

  try {
    // Get weather data from openweathermap.org 
    //  => saved to file [location] (function argument)
    downloadWeatherData(location); 

    ptree dataTree;
    read_json(location, dataTree);

    cleanUpTempFile(location);   

    // check whether child excists
    boost::optional<ptree& > child = dataTree.get_child_optional( "name" );
    if (!child)
      return;

    // Get the longitude from dataTree
    _name = dataTree.get<std::string>("name");

    _longitude = dataTree.get<std::string>("coord.lon");
    _latitude = dataTree.get<std::string>("coord.lat");
    _contry = dataTree.get<std::string>("sys.country");
    _sunrise = dataTree.get<int>("sys.sunrise");
    _sunset = dataTree.get<int>("sys.sunset");

    //TODO Only weather description or main-weather:description?
    _weather = dataTree.get<std::string>("weather..description");

    _temp = dataTree.get<double>("main.temp");
    _tempMin = dataTree.get<double>("main.temp_min");
    _tempMax = dataTree.get<double>("main.temp_max");
    _pressure = dataTree.get<double>("main.pressure");
    _humidity = dataTree.get<double>("main.humidity");

    _windSpeed = dataTree.get<double>("wind.speed");
    _windDirection = dataTree.get<double>("wind.deg");
  } catch(std::exception &e) {
    cout<<e.what();
  }
  
}

void WeatherData::setComparisonFunction(std::function<bool (const WeatherData& lhs, const WeatherData& rhs)> argFunction) {
  priorTo = argFunction;
}

bool WeatherData::excists() const {
  if (_name == "") {
    return false;
  } else {
    return true;
  }
}

void WeatherData::printData() const
{
  cout<<"Location:        \t"   <<GREYTEXT <<_name <<DEFAULTCOLOR <<'\n'
      <<"Coordinates:     \t"  <<coordinates() <<'\n'
      <<"Sunrise/sunset:  \t"   <<_sunrise <<"\t/\t" <<_sunset <<'\n'
      <<"Weather/humidity:\t"   <<_weather <<"\t/\t" <<_humidity <<'\n'
      <<"Temperature:     \t"   <<_temp <<'\n'
      <<"\tmin/max:\t"          <<_tempMin <<"\t\t/\t" <<_tempMax <<'\n'
      <<"Windspeed/direction:\t" <<_windSpeed <<"\t\t/\t" <<_windDirection <<'\n';  
}

