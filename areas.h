#ifndef AREAS_H
#define AREAS_H

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <979961>

  This file contains the Areas class, which is responsible for parsing data
  from a standard input stream and converting it into a series of objects:

  Measure       — Represents a single measure for an area, e.g.
   |              population. Contains a human-readable label and a map of
   |              the measure accross a number of years.
   |
   +-> Area       Represents an area. Contains a unique local authority code
        |         used in national statistics, a map of the names of the area 
        |         (i.e. in English and Welsh), and a map of various Measure 
        |         objects.
        |
        +-> Areas A class that contains all Area objects.

  TODO: Read the block comments with TODO in areas.cpp to know which
  functions and member variables you need to declare in this class.
 */

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>


#include "datasets.h"
#include "area.h"
#include "Helper.h"

/*
  An alias for filters based on strings such as categorisations e.g. area,
  and measures.
*/
using StringFilterSet = std::unordered_set<std::string>;

/*
  An alias for a year filter.
*/
using YearFilterTuple = std::tuple<unsigned int, unsigned int>;

/*
  An alias for the data within an Areas object stores Area objects.

  TODO: you should remove the declaration of the Null class below, and set
  areas to a valid Standard Library container of your choosing.
*/
using AreasContainer = std::map<std::string, Area>;

/*
  Areas is a class that stores all the data categorised by area. The 
  underlying Standard Library container is customisable using the alias above.

  To understand the functions declared below, read the comments in areas.cpp
  and the coursework worksheet. Briefly: populate() is called by bethyw.cpp to
  populate data inside an Areas instance. This function will hand off the
  specific parsing of code to other functions, based on the value of 
  BethYw::SourceDataType.

  TODO: Based on your implementation, there may be additional constructors
  or functions you implement here, and perhaps additional operators you may wish
  to overload.

  !!All of the default values for the parameters are the respective types'
  no filter values.
*/

//in case there is only one name and not a list of names
bool filterCheck(const StringFilterSet * const filterSet,
                 std::string const &code,
                 std::string const &name);

bool filterCheck(const StringFilterSet * const filterSet,
                 std::string const &code,
                 std::unordered_map<std::string, std::string> const &names);

bool yearFilterCheck(const YearFilterTuple * const yearsFilter, const int &year);

class Areas {
private:
    AreasContainer areas;
public:
  Areas() = default;

  int size() const;

  void setArea(std::string const &localAuthorityCode, Area const &area);
  Area getArea(std::string const &localAuthorityCode) const;

  AreasContainer getAreas() const;

  void populateFromAuthorityCodeCSV(
      std::istream& is,
      const BethYw::SourceColumnMapping& cols,
      const StringFilterSet * const areas = new StringFilterSet)
      noexcept(false);


  void populateFromAuthorityByYearCSV(
          std::istream &is,
          const BethYw::SourceColumnMapping &cols,
          const StringFilterSet * const areasFilter = new StringFilterSet(),
          const StringFilterSet * const measuresFilter = new StringFilterSet(),
          const YearFilterTuple * const yearsFilter = new YearFilterTuple(0, 0));

  void populate(
          std::istream& is,
          const BethYw::SourceDataType& type,
          const BethYw::SourceColumnMapping& cols) noexcept(false);

    /*
     * areasFilter, measureFilter and yearsFilter have default values
     * to make sure the code does not break in case the filters passed are nullpointers.
     * In the case of this whole project, this is impossible, as bethyw is carefully written, but, in the real world, new classes
     * always pop up written by different people who do not know the limitations of the code.
     * As such, this is simply a fail-proof in case a developer misuses the code when this script is reused.
     */
  void populate(
      std::istream& is,
      const BethYw::SourceDataType& type,
      const BethYw::SourceColumnMapping& cols,
      const StringFilterSet * const areasFilter = new StringFilterSet(),
      const StringFilterSet * const measuresFilter = new StringFilterSet(),
      const YearFilterTuple * const yearsFilter = new YearFilterTuple(0, 0))
      noexcept(false);

  void populateFromWelshStatsJSON(
          std::istream &is,
          const BethYw::SourceColumnMapping &cols,
          const StringFilterSet * const areasFilter = new StringFilterSet(),
          const StringFilterSet * const measuresFilter = new StringFilterSet(),
          const YearFilterTuple * const yearsFilter = new YearFilterTuple(0, 0));

  friend std::ostream& operator<<(std::ostream& os, const Areas &areas);

  std::string toJSON() const;
};

#endif // AREAS_H