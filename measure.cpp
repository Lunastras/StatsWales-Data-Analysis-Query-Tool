


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <979961>

  This file contains the implementation of the Measure class. Measure is a
  very simple class that needs to contain a few member variables for its name,
  codename, and a Standard Library container for data. The data you need to 
  store is values, organised by year. I'd recommend storing the values as 
  doubles.

  This file contains numerous functions you must implement. Each function you
  must implement has a TODO block comment. 
*/

#include <stdexcept>
#include <string>

#include "measure.h"

/*
  TODO: Measure::Measure(codename, label);

  Construct a single Measure, that has values across many years.

  All StatsWales JSON files have a codename for measures. You should convert 
  all codenames to lowercase.

  @param codename
    The codename for the measure

  @param label
    Human-readable (i.e. nice/explanatory) label for the measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);
*/
Measure::Measure(std::string const &codename, const std::string &label) {
  setLabel(label);

  this->codename = lowerString(codename);
  earliestYear = 9999;
  latestYear = 0;
}

Measure::Measure() {
    earliestYear = 9999;
    latestYear = 0;
}
/*
  TODO: Measure::getCodename()

  Retrieve the code for the Measure. This function should be callable from a 
  constant context and must promise to not modify the state of the instance or 
  throw an exception.

  @return
    The codename for the Measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto codename2 = measure.getCodename();
*/

std::string Measure::getCodename() const{
    return codename;
}

/*
  TODO: Measure::getLabel()

  Retrieve the human-friendly label for the Measure. This function should be 
  callable from a constant context and must promise to not modify the state of 
  the instance and to not throw an exception.

  @return
    The human-friendly label for the Measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto label = measure.getLabel();
*/

std::string Measure::getLabel() const{
    return label;
}



/*
  TODO: Measure::setLabel(label)

  Change the label for the Measure.

  @param label
    The new label for the Measure

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    ...
    measure.setLabel("New Population");
*/

void Measure::setLabel(std::string const &lbl) {
    label = lbl;
}

/*
  TODO: Measure::getValue(key)

  Retrieve a Measure's value for a given year.

  @param key
    The year to find the value for

  @return
    The value stored for the given year

  @throws
    std::out_of_range if year does not exist in Measure with the message
    No value found for year <year>

  @return
    The value

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto value = measure.getValue(1999); // returns 12345678.9
*/

MeasureDataType Measure::getValue(int key) const {

    auto area = data.find(key);

    if(area == data.end()) {
        throw std::out_of_range("The key '" + std::to_string(key) + "' was not found.");
    }

    return data.at(key);
}

/*
 * Get the measures container
 * This function exists only for the Area class to combine the two
 * @return The container with the labels and respective values
 */
std::map<int, MeasureDataType> Measure::getData() const {
    return data;
}

/*
  TODO: Measure::setValue(key, value)

  Add a particular year's value to the Measure object. If a value already
  exists for the year, replace it.

  @param key
    The year to insert a value at

  @param value
    The value for the given year

  @return
    void

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
*/

void Measure::setValue(int key, MeasureDataType value) {
    if(key > latestYear) {
        latestYear = key;
    }

    if(key < earliestYear) {
        earliestYear = key;
    }

    data[key] = value;
    (void)getValue(key); //suppresses warning that the function isn't called
}

/*
  TODO: Measure::size()

  Retrieve the number of years data we have for this measure. This function
  should be callable from a constant context and must promise to not change
  the state of the instance or throw an exception.

  @return
    The size of the measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    auto size = measure.size(); // returns 1
*/

int Measure::size() const {
    return data.size();
}


/*
  TODO: Measure::getDifference()

  Calculate the difference between the first and last year imported. This
  function should be callable from a constant context and must promise to not
  change the state of the instance or throw an exception.

  @return
    The difference/change in value from the first to the last year, or 0 if it
    cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    measure.setValue(1999, 12345679.9);
    auto diff = measure.getDifference(); // returns 1.0
*/

double Measure::getDifference() const{

    double difference = 0;
    if(!data.empty()) {
        difference = data.at(latestYear) - data.at(earliestYear);
    }

    return difference;
}

/*
  TODO: Measure::getDifferenceAsPercentage()

  Calculate the difference between the first and last year imported as a 
  percentage. This function should be callable from a constant context and
  must promise to not change the state of the instance or throw an exception.

  @return
    The difference/change in value from the first to the last year as a decminal
    value, or 0 if it cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1990, 12345678.9);
    measure.setValue(2010, 12345679.9);
    auto diff = measure.getDifferenceAsPercentage();
*/

double Measure::getDifferenceAsPercentage() const{

    double percentage = 0;
    if(!data.empty()) {

        percentage = 100.0f *((double)getDifference() / (double)data.at(earliestYear));
    }

    return percentage;
}

/*
  TODO: Measure::getAverage()

  Calculate the average/mean value for all the values. This function should be
  callable from a constant context and must promise to not change the state of 
  the instance or throw an exception.

  @return
    The average value for all the years, or 0 if it cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    measure.setValue(1999, 12345679.9);
    auto diff = measure.getDifference(); // returns 1
*/

double Measure::getAverage() const {

    double totalValues = 0;

    for(std::pair<int, MeasureDataType> element: data) {
        totalValues += element.second;
    }

    return totalValues / (double)data.size();
}


/*
  TODO: operator<<(os, measure)

  Overload the << operator to print all of the Measure's imported data.

  We align the year and value outputs by padding the outputs with spaces,
  i.e. the year and values should be right-aligned to each other so they
  can be read as a table of numerical values.

  Years should be printed in chronological order. Three additional columns
  should be included at the end of the output, correspodning to the average
  value across the years, the difference between the first and last year,
  and the percentage difference between the first and last year.

  If there is no data in this measure, print the name and code, and 
  on the next line print: <no data>

  See the coursework specification for more information.

  @param os
    The output stream to write to

  @param measure
    The Measure to write to the output stream

  @return
    Reference to the output stream

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    std::cout << measure << std::end;
*/

std::ostream& operator<<(std::ostream& os, Measure const &measure) {

    int const width = 15;
    int const precision = 6;
    os << std::fixed;
    os << std::setprecision(6);
    os << '\n' + measure.getLabel() + " (" + measure.getCodename() + ")\n";

    if(measure.size() > 0) {

        //I go through the data twice because I have no idea how to return to
        //a line in c++, and if my research yielded correct information,
        //it is impossible.

        for(std::pair<int, MeasureDataType> element: measure.getData()) {
            os.width(width);
            os << std::right << element.first;
        }

        os.width(width);
        os << std::right << "Average";

        os.width(width);
        os << std::right << "Diff.";

        os.width(width);
        os << std::right << "% Diff.\n";

        for(std::pair<int, MeasureDataType> element: measure.getData()) {
            os.width(width);
            os << std::right << std::setprecision(precision) << element.second;
        }

        os.width(width);
        os << std::right << std::setprecision(precision) << measure.getAverage();

        os.width(width);
        os << std::right << std::setprecision(precision) << measure.getDifference();

        os.width(width);
        os << std::right << std::setprecision(precision) << measure.getDifferenceAsPercentage();

    } else {
        os << "<no data>";
    }

    os << '\n';

    return os;
}

/*
  TODO: operator==(lhs, rhs)

  Overload the == operator for two Measure objects. Two Measure objects
  are only equal when their codename, label and data are all equal.

  @param lhs
    A Measure object

  @param rhs
    A second Measure object

  @return
    true if both Measure objects have the same codename, label and data; false
    otherwise
*/

bool operator==(Measure const &lhs, Measure const &rhs) {
    if(lhs.getCodename() != rhs.getCodename())
        return false;

    if(lhs.getLabel() != rhs.getLabel())
        return false;

    if(lhs.size() != rhs.size())
        return false;

    for(std::pair<int, MeasureDataType> element: rhs.data) {

        //look for the key in lhs's data and check that the values
        //in both are equal.
        if(lhs.data.count(element.first)) {
            if(lhs.data.at(element.first) != element.second)
                return false;
        } else {
            return false;
        }
    }

    //if we got here then they are equal
    return true;
}

