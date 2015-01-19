Weather app for linux
=====================

## Synopsis

This project is a quick implementation of a command line interface to the current-weather page from open **openweathermap.org**.

## Required system / software

The implementation currently uses **wget** shell software that is preinstalled on most linux systems. 

**wget** can also be installed manually on unix systems

## Compile software

Boost **property_tree** librariy must be installed to compile the software.

The source code is written in C++11, and uses threads. Write **make** to compile..

## Use

The executable accepts the following arguments:
  * -t  Sort results by Temperature
  * -p  Sort results by Pressure 
  * -h  Sort results by Humidity
  * -l  Sort results by Location (default)

Other sort options can easily be made available by implementing the sort lambda funciton under the desired shell argument in **shell::readArgument(int, char* *)** in systemsInterface.cpp

