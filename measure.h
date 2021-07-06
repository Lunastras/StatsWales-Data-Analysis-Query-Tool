#ifndef MEASURE_H_
#define MEASURE_H_

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <979961>

  This file contains the declaration of the Measure class.

  TODO: Read the block comments with TODO in measure.cpp to know which 
  functions and member variables you need to declare in this class.
 */

#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include<stdio.h>
#include "Helper.h"

/*
  The Measure class contains a measure code, label, and a container for readings
  from across a number of years.

  TODO: Based on your implementation, there may be additional constructors
  or functions you implement here, and perhaps additional operators you may wish
  to overload.
*/

using MeasureDataType = double;

class Measure {
private:
    std::string label;
    std::string codename;
    std::map<int, MeasureDataType> data;
    int earliestYear;
    int latestYear;

public:
    Measure();
    Measure(std::string const &code, const std::string &label);

    std::string getLabel() const;
    std::string getCodename() const;
    void setLabel(std::string const &label);
    MeasureDataType getValue(int key) const;
    void setValue(int key, MeasureDataType value);
    int size() const;
    double getDifference() const;
    double getDifferenceAsPercentage() const;
    double getAverage() const;
    std::map<int, MeasureDataType> getData() const;


    friend bool operator==(Measure const &lhs, Measure const &rhs);
    friend std::ostream& operator<<(std::ostream& os,  Measure const &measure) ;


};

#endif // MEASURE_H_