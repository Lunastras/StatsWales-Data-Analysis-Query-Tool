#ifndef AREA_H_
#define AREA_H_

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <979961>

  This file contains the Area class declaration. Area objects contain all the
  Measure objects for a given local area, along with names for that area and a
  unique authority code.

  TODO: Read the block comments with TODO in area.cpp to know which 
  functions and member variables you need to declare in this class.
 */

#include <string>
#include <unordered_map>

#include "measure.h"
#include "Helper.h"

/*
  An Area object consists of a unique authority code, a container for names
  for the area in any number of different languages, and a container for the
  Measures objects.

  TODO: Based on your implementation, there may be additional constructors
  or functions you implement here, and perhaps additional operators you may wish
  to overload.
*/
class Area {
public:

    Area() = default;
    Area(const std::string& localAuthorityCode);
    std::string getLocalAuthorityCode() const;
    std::string getName(std::string const &lang) const;
    void setName(std::string lang, std::string const &name);
    std::unordered_map<std::string, std::string> getNamesList() const;
    std::map<std::string, Measure> getMeasuresList() const;
    Measure getMeasure(std::string key) const;
    void setMeasure(std::string key, Measure const &measure);

    friend std::ostream& operator<<(std::ostream& os, Area const &area);
    friend bool operator==(Measure const &lhs, Measure const &rhs);

    int size() const;

private:
    std::unordered_map<std::string, std::string> names;
    std::string authorityCode;
    std::map<std::string, Measure> measures;
};

#endif // AREA_H_