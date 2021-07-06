


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <979961>

  The file contains the Areas class implementation. Areas are the top
  level of the data structure in Beth Yw? for now.

  Areas is also responsible for importing data from a stream (using the
  various populate() functions) and creating the Area and Measure objects.

  This file contains numerous functions you must implement. Each function you
  must implement has a TODO block comment. 
*/

#include <stdexcept>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>

#include "lib_json.hpp"

#include "datasets.h"
#include "areas.h"

/*
  An alias for the imported JSON parsing library.
*/
using json = nlohmann::json;

/*
  TODO: Areas::setArea(localAuthorityCode, area)

  Add a particular Area to the Areas object.

  If an Area already exists with the same local authority code, overwrite all
  data contained within the existing Area with those in the new
  Area (i.e. they should be combined, but the new Area's data should take
  precedence, e.g. replace a name with the same language identifier).

  @param localAuthorityCode
    The local authority code of the Area

  @param area
    The Area object that will contain the Measure objects

  @return
    void

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
*/
void Areas::setArea(std::string const &localAuthorityCode, Area const &area) {

    auto currentArea = areas.find(localAuthorityCode);
     //std::cout<<"\n\n-----------set area called with: --------------\n\n";

    if(currentArea != areas.end()) {

        //std::cout << areas[localAuthorityCode];
        for(std::pair<std::string, std::string> element : area.getNamesList()) {
            //first = key
            //second = value
            areas[localAuthorityCode].setName(element.first, element.second);
        }

        for(std::pair<std::string, Measure> element : area.getMeasuresList()) {
            //first = key
            //second = value
            areas[localAuthorityCode].setMeasure(element.first, element.second);
        }

       // std::cout<<areas[localAuthorityCode];
    } else {
        areas[localAuthorityCode] = area;
    }

    //suppresses warning that the function isn't called
    (void)getArea(localAuthorityCode);

   // std::cout<<"\n\n-----------set area END --------------\n\n";
}

/*
  TODO: Areas::getArea(localAuthorityCode)

  Retrieve an Area instance with a given local authority code.

  @param localAuthorityCode
    The local authority code to find the Area instance of

  @return
    An Area object

  @throws
    std::out_of_range if an Area with the set local authority code does not
    exist in this Areas instance

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
    ...
    Area area2 = areas.getArea("W06000023");
*/

Area Areas::getArea(std::string const &localAuthorityCode) const {

    auto area = areas.find(localAuthorityCode);

    if(area == areas.end()) {
        throw std::out_of_range("The key '" + localAuthorityCode + "' was not found.");
    }

    return areas.at(localAuthorityCode);
}


/*
  TODO: Areas::size()

  Retrieve the number of Areas within the container. This function should be 
  callable from a constant context, not modify the state of the instance, and
  must promise not throw an exception.

  @return
    The number of Area instances

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
    
    auto size = areas.size(); // returns 1
*/

int Areas::size() const {

    return areas.size();
}

/*
  TODO: Areas::populateFromAuthorityCodeCSV(is, cols, areasFilter)

  This function specifically parses the compiled areas.csv file of local
  authority codes, and their names in English and Welsh.

  This is a simple dataset that is a comma-separated values file (CSV), where
  the first row gives the name of the columns, and then each row is a set of
  data.

  For this coursework, you can assume that areas.csv will always have the same
  three columns in the same order.

  Once the data is parsed, you need to create the appropriate Area objects and
  insert them in to a Standard Library container within Areas.

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @return
    void

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/areas.csv");
    auto is = input.open();

    auto cols = InputFiles::AREAS.COLS;

    auto areasFilter = BethYw::parseAreasArg();

    Areas data = Areas();
    areas.populateFromAuthorityCodeCSV(is, cols, &areasFilter);

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols
*/
void Areas::populateFromAuthorityCodeCSV(
    std::istream &is,
    const BethYw::SourceColumnMapping &cols,
    const StringFilterSet * const areasFilter) {
   // std::cout<< "haha populateFromAuthorityCodeCSV was called \n";

    std::string line;
    std::getline(is, line); //get rid of the first line

    while(!is.eof()) {

        std::getline(is, line);
        std::string::size_type split;
        std::vector<std::string> elements;

        while((split = line.find(',')) != std::string::npos) {
            elements.push_back(line.substr(0, split));
            line = line.erase(0, split + 1);
        }

        elements.push_back(line);

        if(elements.size() != 3) {
            throw std::out_of_range("Number of arguments not compatible");
        }

        std::string authorityCode = elements.at(0);
        std::string engName = elements.at(1);
        std::string cymName = elements.at(2);

        Area area(authorityCode);
        area.setName("eng", engName);
        area.setName("cym", cymName);

        //check if it is found in filter and if a filter exists
        if(filterCheck(areasFilter, authorityCode, area.getNamesList())) {
            setArea(authorityCode, area);
           // std::cout<< authorityCode + ", " + area.getName("eng") + ", " + area.getName("cym") << std::endl;
        }
    } // while(!is.eof())
}

/**
 * Check if the value is within the filter or if there is a filter
 * @param yearsFilter The filter
 * @param year The value to check
 * @return True if the year is within the two years in the filter or if the filter is {0, 0},
 * false otherwise
 */
bool yearFilterCheck(const YearFilterTuple * const yearsFilter, const int &year) {

    int earliestYear = std::get<0>(*yearsFilter);
    int latestYear = std::get<1>(*yearsFilter);

   // std::cout<< "tuple is: <" << earliestYear << "," << latestYear <<"> and the value is " << year << "\n\n";
    if(earliestYear != 0) {
        if(latestYear != 0) {
            return (year >= earliestYear) && (year <= latestYear);
        } else {
            return year == earliestYear;
        }
    } else {
        return true;
    }
}

/**
 * Check if the code and names are within the filter or if there is a filter
 * @param filterSet The filter
 * @param code The code to check
 * @param name Names of the object searched for
 * @return True if the code is in the filter or if the filter is empty, false otherwise.
 */
bool filterCheck(const StringFilterSet * const filterSet,
                 std::string const &code,
                 std::unordered_map<std::string, std::string> const &names) {


    //we lower the letters in order to make the
    //filter case insensitive (e.g. swan = SWAN)
    //all of the values in the filter are already small case;
    std::string lwrCode = lowerString(code);
    std::vector<std::string> lwrNames;

    for(std::pair<std::string, std::string> name : names) {
        lwrNames.push_back(lowerString(name.second));
    }

    if(filterSet->empty()){
        //std::cout<<"this is where " << names.at("eng") << " passed 2\n\n";
        //if the filter doesn't exist
        return true;
    } else {
        //if the value of the exact code is found in the filter
        if(filterSet->find(code) != filterSet->end()) {
           // std::cout<<"this is where " << names.at("eng") << " passed 3\n\n";

            return true;
        } else {

            for(const std::string &filterName : *filterSet) {
                if(lwrCode.find(filterName) != std::string::npos) {
                 //   std::cout<<"this is where " << names.at("eng") << " with " << filterName << " and " << lwrCode << " passed 4\n\n ";
                    return true;
                } else {
                    for(const std::string &name : lwrNames) {
                        if(name.find(filterName) != std::string::npos) {
                            //std::cout<<"this is where " << names.at("eng") << " passed 5\n\n";
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

/**
 * Check if the code and name are within the filter or if there is a filter
 * @param filterSet The filter
 * @param code The code to check
 * @param name Name of the object searched for
 * @return True if the code is in the filter or if the filter is empty, false otherwise.
 */
bool filterCheck(const StringFilterSet * const filterSet,
                 std::string const &code,
                 std::string const &name){

    //we lower the letters in order to make the
    //filter case insensitive (e.g. swan = SWAN)
    //all of the values in the filter are already small case;
    std::string lwrCode = lowerString(code);
    std::string lwrName = lowerString(name);

    if(filterSet->empty()){
        //if the filter doesn't exist

        return true;
    } else {
        //if the value of the exact code or name is found in the filter
        if((filterSet->find(code) != filterSet->end())
           || filterSet->find(name) != filterSet->end()) {
            return true;
        } else {
            for(const std::string &filterName : *filterSet) {
                if(lwrCode.find(filterName) != std::string::npos ||
                !lwrName._Equal("") && (lwrName.find(filterName) != std::string::npos)) {
                    return true;
                }
            }
        }
    }
    return false;
}

/*
  TODO: Areas::populateFromWelshStatsJSON(is,
                                          cols,
                                          areasFilter,
                                          measuresFilter,
                                          yearsFilter)

  Data from StatsWales is in the JSON format, and contains three
  top-level keys: odata.metadata, value, odata.nextLink. value contains the
  data we need. Rather than been hierarchical, it contains data as a
  continuous list (e.g. as you would find in a table). For each row in value,
  there is a mapping of various column headings and their respective values.

  Therefore, you need to go through the items in value (in a loop)
  using a JSON library. To help you, I've selected the nlohmann::json
  library that you must use for your coursework. Read up on how to use it here:
  https://github.com/nlohmann/json

  Example of using this library:
    - Reading/parsing in from a stream is very simply using the >> operator:
        json j;
        stream >> j;

    - Looping through parsed JSON is done with a simple for each loop. Inside
      the loop, you can access each using the array syntax, with the key/
      column name, e.g. data["Localauthority_ItemName_ENG"] gives you the
      local authority name:
        for (auto& el : j["value"].items()) {
           auto &data = el.value();
           std::string localAuthorityCode = data["Localauthority_ItemName_ENG"];
           // do stuff here...
        }

  In this function, you will have to parse the JSON datasets, extracting
  the local authority code, English name (the files only contain the English
  names), and each measure by year.

  If you encounter an Area that does not exist in the Areas container, you
  should create the Area object

  If areasFilter is a non-empty set only include areas matching the filter. If
  measuresFilter is a non-empty set only include measures matching the filter.
  If yearsFilter is not equal to <0,0>, only import years within the range
  specified by the tuple (inclusive).

  I've provided the column names for each JSON file that you need to parse
  as std::strings in datasets.h. This mapping should be passed through to the
  cols parameter of this function.

  Note that in the JSON format, years are stored as strings, but we need
  them as ints. When retrieving values from the JSON library, you will
  have to cast them to the right type.

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings of areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of umodifiable strings of measures to import,
    or an empty set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as the range of years to be imported (inclusively)

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    areas.populateFromWelshStatsJSON(
      is,
      cols,
      &areasFilter,
      &measuresFilter,
      &yearsFilter);
*/

void Areas::populateFromWelshStatsJSON(
        std::istream &is,
        const BethYw::SourceColumnMapping &cols,
        const StringFilterSet * const areasFilter,
        const StringFilterSet * const measuresFilter,
        const YearFilterTuple * const yearsFilter) {

    //std::cout<<"I guess json welsh stats was called... bepis\n";
    json j;

    is >> j;

    std::string authCode;
    std::string engName;
    std::string cymName;
    double measureValue;
    int year;
    std::string measureName;
    std::string measureCode;

    for (auto& el : j["value"].items()) {
        auto &data = el.value();

        authCode = "";
        engName = "";
        cymName = "";
        measureValue = 0;
        year = 0;
        measureName = "";
        measureCode = "";

        for(const std::pair<BethYw::SourceColumn , std::string> elem : cols) {

            auto value = data[cols.at(elem.first)];
            if(!value.is_null()) {

                switch(elem.first) {
                    case BethYw::SourceColumn::SINGLE_MEASURE_CODE:
                        measureCode = value;
                       // std::cout<< elem.second << ": " << measureCode << ", ";
                        break;

                    case BethYw::SourceColumn::AUTH_CODE:
                        authCode = value;
                       // std::cout<< elem.second << ": " << authCode << ", ";
                        break;

                    case BethYw::SourceColumn::AUTH_NAME_CYM:
                        cymName = value;
                       // std::cout<< elem.second << ": " << cymName << ", ";
                        break;

                    case BethYw::SourceColumn::AUTH_NAME_ENG:
                        engName = value;
                       // std::cout<< elem.second << ": " << engName << ", ";
                        break;

                    case BethYw::SourceColumn::MEASURE_CODE:
                        measureCode = value;
                        //std::cout<< elem.second << ": " << measureCode << ", ";
                        break;
                    /*
                     * I just found out you can do this to sort of do an 'or' statement
                     * in a switch.
                     * This is what I like to call a pro gamer move.
                     */
                    case BethYw::SourceColumn::MEASURE_NAME:
                    case BethYw::SourceColumn::SINGLE_MEASURE_NAME:
                        measureName = value;
                       // std::cout<< elem.second << ": " << measureName << ", ";
                        break;

                    case BethYw::SourceColumn::VALUE:
                        measureValue = value;
                      //  std::cout<< elem.second << ": " << measureValue << ", ";
                        break;

                    case BethYw::SourceColumn::YEAR:
                        year = std::stoi((std::string)value);
                      //  std::cout<< elem.second << ": " << year << ", ";
                        break;

                    default:
                        break;

                }
            }

        } //end for 2

        Measure measure(measureCode, measureName);
        measure.setValue(year, measureValue);
        Area area(authCode);

        if(!engName._Equal("")) {
            area.setName("eng", engName);
        }

        if(!cymName._Equal("")) {
            area.setName("cym", cymName);
        }

        if(filterCheck(areasFilter, area.getLocalAuthorityCode(), area.getNamesList())) {
            if(filterCheck(measuresFilter, measureCode, measureName)) {
                if(yearFilterCheck(yearsFilter, year)) {
                    // std::cout<< authCode << " " << engName << " " << cymName << " " <<
                    //measureName << " ("<< measureCode  << ") "<< measureValue  << " "<< year << "\n\n";

                    area.setMeasure(measure.getCodename(), measure);
                    setArea(authCode, area);
                } // if(yearFilterCheck(yearsFilter, year))
            } //end if measureFilter
        } //end if areasFilter
    } //end  for (auto& el : j["value"].items())

    //std::cout<<"\n\n*************FINISHED READING FILE***************\n\n";
}

/*
  TODO: Areas::populateFromAuthorityByYearCSV(is,
                                              cols,
                                              areasFilter,
                                              yearFilter)

  This function imports CSV files that contain a single measure. The 
  CSV file consists of columns containing the authority code and years.
  Each row contains an authority code and values for each year (or no value
  if the data doesn't exist).

  Note that these files do not include the names for areas, instead you
  have to rely on the names already populated through 
  Areas::populateFromAuthorityCodeCSV();

  The datasets that will be parsed by this function are
   - complete-popu1009-area.csv
   - complete-popu1009-pop.csv
   - complete-popu1009-opden.csv

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of strings for measures to import, or an empty 
    set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as a the range of years to be imported

  @return
    void

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/complete-popu1009-pop.csv");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["complete-pop"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto yearsFilter = BethYw::parseYearsArg();

    Areas data = Areas();
    areas.populateFromAuthorityCodeCSV(is, cols, &areasFilter, &yearsFilter);

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols

*/

void Areas::populateFromAuthorityByYearCSV(
        std::istream &is,
        const BethYw::SourceColumnMapping &cols,
        const StringFilterSet * const areasFilter,
        const StringFilterSet * const measuresFilter,
        const YearFilterTuple * const yearsFilter) {
   // std::cout<< "haha populateFromAuthorityByYearCSV was called \n";


    std::string line;
    std::vector<int> years; //we will store the years in this vector

    //parse the years on the first line
    std::getline(is, line);
    std::string::size_type split = line.find(',');
    line = line.erase(0, split + 1);

    while((split = line.find(',')) != std::string::npos) {
        years.push_back(std::stoi(line.substr(0, split)));
        line = line.erase(0, split + 1);
    }

    //add the final year to the vector
    years.push_back(std::stoi(line));

    while(!is.eof()) {

        std::getline(is, line);
        std::vector<double> elements;
        split = line.find(',');

        //fetch the authority code
        std::string authorityCode = line.substr(0, split);
        line = line.erase(0, split + 1);

        //if area is in filter
        //because there is no name for these values, we will only add the empty string
        if(filterCheck(areasFilter, authorityCode, "")) {

            std::string measureCode = cols.at(BethYw::SINGLE_MEASURE_CODE);
            std::string measureName = cols.at(BethYw::SINGLE_MEASURE_NAME);

            if(filterCheck(measuresFilter, measureCode, measureName)) {
                split = line.find(',');

                //throw exception if there are no commas
                if(split == std::string::npos) {
                    throw std::out_of_range("Not enough columns in the data");
                }

                Area area(authorityCode);
                Measure measure(measureCode,
                                measureName);

                for(int i = 0; i < years.size(); i++) {

                    int currentYear = years.at(i);

                    //check if year is in filter or if the filter exists
                    if(yearFilterCheck(yearsFilter, currentYear)) {

                        /*
                         * Check if there are any more values in the columns
                         * if not, make sure the number of columns is at the very least
                         * equal to the number of years.
                         * With this code, any extra values will simply be ignored,
                         * and it will throw an exception if the number of columns
                         * is too small.
                         */
                        std::string stringValue;

                        if(split != std::string::npos) { //check if there is a comma
                            stringValue = line.substr(0, split);
                        } else if(i == (years.size() - 1)) { //check if we are at the last value
                            stringValue = line;
                        } else {
                            //if no comma is found and there are more
                            // than one values left, throw exception
                            throw std::out_of_range("Not enough columns in the data");
                        }

                        double value = std::stod(stringValue);
                        measure.setValue(currentYear, value);
                    }

                    //cut to the next element
                    if(split != std::string::npos) {
                        line = line.erase(0, split + 1);
                        split = line.find(',');
                    }
                } // end for(int i = 0; i < years.size(); i++)

                area.setMeasure(measure.getCodename(), measure);
                setArea(authorityCode, area);
            } // end if(filterCheck(measuresFilter, measureCode))
        } // end if(filterCheck(areasFilter, authorityCode))
    }//end while(!is.eof())
}

/*
  TODO: Areas::populate(is, type, cols)

  Parse data from an standard input stream `is`, that has data of a particular
  `type`, and with a given column mapping in `cols`.

  This function should look at the `type` and hand off to one of the three
  functions populate………() functions.

  The function must check if the stream is in working order and has content.

  @param is
    The input stream from InputSource

  @param type
    A value from the BethYw::SourceDataType enum which states the underlying
    data file structure

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @return
    void

  @throws
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file),
    the stream is not open/valid/has any contents, or an unexpected type
    is passed in.
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of the values variable type can have

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    Areas data = Areas();
    areas.populate(
      is,
      DataType::WelshStatsJSON,
      cols);
*/
void Areas::populate(std::istream &is,
                     const BethYw::SourceDataType &type,
                     const BethYw::SourceColumnMapping &cols) {
    if (type == BethYw::AuthorityCodeCSV) {
        populateFromAuthorityCodeCSV(is, cols);
    } else if(type == BethYw::AuthorityByYearCSV){

        populateFromAuthorityByYearCSV(
                is,
                cols);

    } else if(type == BethYw::WelshStatsJSON) {

        populateFromWelshStatsJSON(
                is,
                cols);
    } else {
        throw std::runtime_error("Areas::populate: Unexpected data type");
    }
}

/*
  TODO: Areas::populate(is,
                        type,
                        cols,
                        areasFilter,
                        measuresFilter,
                        yearsFilter)

  Parse data from an standard input stream, that is of a particular type,
  and with a given column mapping, filtering for specific areas, measures,
  and years, and fill the container.

  This function should look at the `type` and hand off to one of the three 
  functions you've implemented above.

  The function must check if the stream is in working order and has content.

  This overloaded function includes pointers to the three filters for areas,
  measures, and years.

  @param is
    The input stream from InputSource

  @param type
    A value from the BethYw::SourceDataType enum which states the underlying
    data file structure

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of umodifiable strings for measures to import,
    or an empty set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as a the range of years to be imported

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file),
    the stream is not open/valid/has any contents, or an unexpected type
    is passed in.
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of the values variable type can have

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variables areasFilter, measuresFilter,
    and yearsFilter are created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    areas.populate(
      is,
      DataType::WelshStatsJSON,
      cols,
      &areasFilter,
      &measuresFilter,
      &yearsFilter);
*/
void Areas::populate(
    std::istream &is,
    const BethYw::SourceDataType &type,
    const BethYw::SourceColumnMapping &cols,
    const StringFilterSet * const areasFilter,
    const StringFilterSet * const measuresFilter,
    const YearFilterTuple * const yearsFilter)
     {
  if (type == BethYw::AuthorityCodeCSV) {
    populateFromAuthorityCodeCSV(is, cols, areasFilter);
  } else if(type == BethYw::AuthorityByYearCSV){

      populateFromAuthorityByYearCSV(
              is,
              cols,
              areasFilter,
              measuresFilter,
              yearsFilter);

  } else if(type == BethYw::WelshStatsJSON) {

      populateFromWelshStatsJSON(
              is,
              cols,
              areasFilter,
              measuresFilter,
              yearsFilter);
  } else {
      throw std::runtime_error("Areas::populate: Unexpected data type");
  }
}

/*
  TODO: Areas::toJSON()

  Convert this Areas object, and all its containing Area instances, and
  the Measure instances within those, to values.

  Use the sample JSON library as above to create this. Construct a blank
  JSON object:
    json j;

  Convert this json object to a string:
    j.dump();

  You then need to loop through your areas, measures, and years/values
  adding this data to the JSON object.

  Read the documentation for how to convert your outcome code to JSON:
    https://github.com/nlohmann/json#arbitrary-types-conversions
  
  The JSON should be formatted as such:
    {
    "<localAuthorityCode1>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               },
    "<localAuthorityCode2>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               }
    ...
    "<localAuthorityCodeN>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               }
    }

  An empty JSON is "{}" (without the quotes), which you must return if your
  Areas object is empty.
  
  @return
    std::string of JSON

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    std::cout << data.toJSON();
*/
std::string Areas::toJSON() const {
  json j;
  
  return j.dump();
}

/**
 * Get the areas container.
 * @return The areas container
 */
AreasContainer Areas::getAreas() const {
    return areas;
}

/*
  TODO: operator<<(os, areas)

  Overload the << operator to print all of the imported data.

  Output should be formatted like the following to pass the tests. Areas should
  be printed, ordered alphabetically by their local authority code. Measures 
  within each Area should be ordered alphabetically by their codename.

  See the coursework specification for more information, although for reference
  here is a quick example of how output should be formatted:

    <English name of area 1> / <Welsh name of area 1> (<authority code 1>)
    <Measure 1 name> (<Measure 1 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 1> <diff 1> <diffp 1>

    <Measure 2 name> (<Measure 2 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 2> <diff 2> <diffp 2>

    <Measure 3 name> (<Measure 3 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 3> <diff 3> <diffp 3>

    ...

    <Measure x name> (<Measure x code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean x> <diff x> <diffp x>


    <English name of area 2> / <Welsh name of area 2> (<authority code 2>)
    <Measure 1 name> (<Measure 1 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 1> <diff 1> <diffp 1>

    <Measure 2 name> (<Measure 2 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 2> <diff 2> <diffp 2>

    <Measure 3 name> (<Measure 3 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 3> <diff 3> <diffp 3>

    ...

    <Measure x name> (<Measure x code>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean x> <diff x> <diffp x>

    ...

    <English name of area y> / <Welsh name of area y> (<authority code y>)
    <Measure 1 name> (<Measure 1 codename>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean 1> <diff 1> <diffp 1>

    <Measure 2 name> (<Measure 2 codename>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean 2> <diff 2> <diffp 2>

    <Measure 3 name> (<Measure 3 codename>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean 3> <diff 3> <diffp 3>

    ...

    <Measure x name> (<Measure x codename>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean x> <diff x> <diffp x>

  With real data, your output should start like this for the command
  bethyw --dir <dir> -p popden -y 1991-1993 (truncated for readability):

    Isle of Anglesey / Ynys Môn (W06000001)
    Land area (area) 
          1991       1992       1993    Average    Diff.  % Diff. 
    711.680100 711.680100 711.680100 711.680100 0.000000 0.000000 

    Population density (dens) 
         1991      1992      1993   Average    Diff.  % Diff. 
    97.126504 97.486216 98.038430 97.550383 0.911926 0.938905 

    Population (pop) 
            1991         1992         1993      Average      Diff.  % Diff. 
    69123.000000 69379.000000 69772.000000 69424.666667 649.000000 0.938906 


    Gwynedd / Gwynedd (W06000002)
    Land area (Area)
    ...

  @param os
    The output stream to write to

  @param areas
    The Areas object to write to the output stream

  @return
    Reference to the output stream

  @example
    Areas areas();
    std::cout << areas << std::end;
*/

std::ostream& operator<<(std::ostream& os, Areas const &areas) {

    if(areas.size() == 0) {
        os << "<No areas>\n";
    } else {
        for(std::pair<std::string, Area> element : areas.getAreas()) {
            os << element.second;
        }
    }
    return os;
}

