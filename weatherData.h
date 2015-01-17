#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <functional>

class WeatherData {
  public:
    explicit WeatherData(std::string location);
    
    bool excists() const;
    void printData() const;
    
    const std::string& name() const        { return _name; }
    const std::string& longitude() const   { return _longitude; }
    std::string coordinates() const        { const std::string ret = "["+_latitude+", " + _longitude + "]"; return ret; }
    const double temperature() const       { return _temp; }
    const double pressure() const          { return _pressure; }

    static void setComparisonFunction(std::function<bool (const WeatherData& lhs, const WeatherData& rhs)> argFunction);
    static std::function<bool (const WeatherData& lhs, const WeatherData& rhs)> comparisonFunction() { return priorTo; }
    
  private:
    void load(const std::string location);

    std::string _name;
    std::string _longitude;
    std::string _latitude;
    std::string _contry;
    int _sunrise;
    int _sunset;
    std::string _weather;   //TODO Only weather description or main-weather:description?
    double _temp;
    double _tempMin;
    double _tempMax;
    double _pressure;
    double _humidity;
    double _windSpeed;
    double _windDirection;

    // Comparison function
    static std::function<bool (const WeatherData& lhs, const WeatherData& rhs)> priorTo;
};

#endif

