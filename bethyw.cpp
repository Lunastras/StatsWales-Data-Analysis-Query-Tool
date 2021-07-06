


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <979961>

  This file contains all the helper functions for initialising and running
  Beth Yw? In languages such as Java, this would be a class, but we really
  don't need a class here. Classes are for modelling data, and so forth, but
  here the code is pretty much a sequential block of code (BethYw::run())
  calling a series of helper functions.

  TODO: This file contains numerous functions you must implement. Each one
  is denoted with a TODO in the block comment. Note that some code has been
  provided in some functions to get you started, but you should read through
  this code and make sure it is safe and complete. You may need to remove or
  modify the provided code to get your program to work fully. You may implement
  additional functions not specified.
*/

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

#include "lib_cxxopts.hpp"

#include "areas.h"
#include "bethyw.h"
#include "input.h"

/*
  Run Beth Yw?, parsing the command line arguments, importing the data,
  and outputting the requested data to the standard output/error.

  Hint: cxxopts.parse() throws exceptions you'll need to catch. Read the cxxopts
  documentation for more information.

  @param argc
    Number of program arguments

  @param argv
    Program arguments

  @return
    Exit code
*/
int BethYw::run(int argc, char *argv[]) {
  auto cxxopts = BethYw::cxxoptsSetup();
  auto args = cxxopts.parse(argc, argv);
  // Print the help usage if requested
  if (args.count("help")) {
    std::cerr << cxxopts.help() << std::endl;
    return 0;
  }

  // Parse data directory argument
  std::string dir = args["dir"].as<std::string>() + DIR_SEP;

  // Parse other arguments and import data
   auto datasetsToImport = BethYw::parseDatasetsArg(args);
   auto areasFilter      = BethYw::parseAreasArg(args);
   auto measuresFilter   = BethYw::parseMeasuresArg(args);
   auto yearsFilter      = BethYw::parseYearsArg(args);

   Areas data = Areas();

   BethYw::loadAreas(data, dir, areasFilter);

   BethYw::loadDatasets(data,
                        dir,
                        datasetsToImport,
                        areasFilter,
                        measuresFilter,
                        yearsFilter);

  if (args.count("json")) {
    // The output as JSON
    std::cout << data.toJSON() << std::endl;
  } else {
   //  The output as tables
    std::cout << data << std::endl;
  }

  return 0;
}

/*
  This function sets up and returns a valid cxxopts object. You do not need to
  modify this function.

  @return
     A constructed cxxopts object

  @example
    auto cxxopts = BethYw::cxxoptsSetup();
    auto args = cxxopts.parse(argc, argv);
*/
cxxopts::Options BethYw::cxxoptsSetup() {
  cxxopts::Options cxxopts(
        "bethyw",
        "Student ID: " + STUDENT_NUMBER + "\n\n"
        "This program is designed to parse official Welsh Government"
        " statistics data files.\n");
    
  cxxopts.add_options()(
      "dir",
      "Directory for input data passed in as files",
      cxxopts::value<std::string>()->default_value("datasets"))(

      "d,datasets",
      "The dataset(s) to import and analyse as a comma-separated list of codes "
      "(omit or set to 'all' to import and analyse all datasets)",
      cxxopts::value<std::vector<std::string>>())(

      "a,areas",
      "The areas(s) to import and analyse as a comma-separated list of "
      "authority codes (omit or set to 'all' to import and analyse all areas)",
      cxxopts::value<std::vector<std::string>>())(

      "m,measures",
      "Select a subset of measures from the dataset(s) "
      "(omit or set to 'all' to import and analyse all measures)",
      cxxopts::value<std::vector<std::string>>())(

      "y,years",
      "Focus on a particular year (YYYY) or "
      "inclusive range of years (YYYY-ZZZZ)",
      cxxopts::value<std::string>()->default_value("0"))(

      "j,json",
      "Print the output as JSON instead of tables.")(

      "h,help",
      "Print usage.");

  return cxxopts;
}


/*
  TODO: BethYw::parseDatasetsArg(args)

  Parse the datasets argument passed into the command line. 

  The datasets argument is optional, and if it is not included, all datasets 
  should be imported. If it is included, it should be a comma-separated list of 
  datasets to import. If the argument contains the value "all"
  (case-insensitive), all datasets should be imported.

  This function validates the passed in dataset names against the codes in
  DATASETS array in the InputFiles namespace in datasets.h. If an invalid code
  is entered, throw a std::invalid_argument with the message:
  No dataset matches key: <input code>
  where <input name> is the name supplied by the user through the argument.

  @param args
    Parsed program arguments

  @return
    A std::vector of BethYw::InputFileSource instances to import

  @throws
    std::invalid_argument if the argument contains an invalid dataset with
    message: No dataset matches key <input code>

  @example
    auto cxxopts = BethYw::cxxoptsSetup();
    auto args = cxxopts.parse(argc, argv);

    auto datasetsToImport = BethYw::parseDatasetsArg(args);
 */
std::vector<BethYw::InputFileSource> BethYw::parseDatasetsArg(
    cxxopts::ParseResult& args) {

    bool importAll = false;
  // This function is incomplete, but to get you started...
  // You may want to delete much of these // comments too!

  // Retrieve all valid datasets, see datasets.h
  size_t numDatasets = InputFiles::NUM_DATASETS;
  auto &allDatasets = InputFiles::DATASETS;

  // Create the container for the return type
  std::vector<InputFileSource> datasetsToImport;

    //try to fetch the datasets.
    if(args.count("datasets")) {
        auto inputDatasets = args["datasets"].as<std::vector<std::string>>();

        //check if the first value is all
        //if so, import all
        if(inputDatasets[0]._Equal("all")) {

            importAll = true;
        } else {
            for(std::string const &element: inputDatasets) {
                bool foundDataset = false;
               // std::cout<<element<< " ";

                for(int j = 0; j < numDatasets; j++) {

                    if(allDatasets[j].CODE._Equal(element)) {
                        datasetsToImport.push_back(allDatasets[j]);
                        foundDataset = true;
                        //break is often considered bad practice
                        //but in the context of iterating through a list
                        //to find an element, it is the objectively
                        //good choice and any arguments against it are invalid
                        break;
                    }
                } //end for 2

                if(!foundDataset) {
                    throw std::invalid_argument("No dataset matches key " + element);
                }
            } //end for 1
        } // end else

    } else {
       // std::cout << "No areas arguments" << std::endl;
        importAll = true;
    }

    //check if all datasets need to be imported.
  if(importAll) {
      for(unsigned int i = 0; i < numDatasets; i++)
          datasetsToImport.push_back(allDatasets[i]);
  }

  return datasetsToImport;
}

/*
  TODO: BethYw::parseAreasArg(args)
  
  Parses the areas command line argument, which is optional. If it doesn't
  exist or exists and contains "all" as value (any case), all areas should be
  imported, i.e., the filter should be an empty set.

  Unlike datasets we can't check the validity of the values as it depends
  on each individual file imported (which hasn't happened until runtime).
  Therefore, we simply fetch the list of areas and later pass it to the
  Areas::populate() function.

  The filtering of inputs should be case insensitive.

  @param args
    Parsed program arguments

  @return 
    An std::unordered_set of std::strings corresponding to specific areas
    to import, or an empty set if all areas should be imported.

  @throws
    std::invalid_argument if the argument contains an invalid areas value with
    message: Invalid input for area argument
*/
StringFilterSet BethYw::parseAreasArg(
    cxxopts::ParseResult& args) {
  // The unordered set you will return
  StringFilterSet areas;

   //check if there are areas arguments
  if(args.count("areas")){
      auto temp = args["areas"].as<std::vector<std::string>>();

      if(!temp[0]._Equal("all")) {
          for(std::string const &element: temp) {

             // std::cout<<element<< " ";
              areas.insert(lowerString(element));
          } //end for
      } //end if
  } else { //debugging purpose
      //std::cout << "No areas arguments" << std::endl;
  }

    //std::cout << std::endl;

  return areas;
}

/*
  TODO: BethYw::parseMeasuresArg(args)

  Parse the measures command line argument, which is optional. If it doesn't 
  exist or exists and contains "all" as value (any case), all measures should
  be imported.

  Unlike datasets we can't check the validity of the values as it depends
  on each individual file imported (which hasn't happened until runtime).
  Therefore, we simply fetch the list of areas and later pass it to the
  Areas::populate() function.

  The filtering of inputs should be case insensitive.

  @param args
    Parsed program arguments

  @return 
    An std::unordered_set of std::strings corresponding to specific measures
    to import, or an empty set if all measures should be imported.

  @throws
    std::invalid_argument if the argument contains an invalid measures value
    with the message: Invalid input for measures argument
*/
StringFilterSet BethYw::parseMeasuresArg(
        cxxopts::ParseResult& args) {
    // The unordered set you will return
    StringFilterSet measures;

    //check if there are measure arguments
    if(args.count("measures")){
        auto temp = args["measures"].as<std::vector<std::string>>();

        if(!temp[0]._Equal("all")) {

            for(std::string const &element: temp) {

               // std::cout<<element<< " ";
                measures.insert(lowerString(element));
            } //end for
        } //end if
    } else { //debugging purpose
        //std::cout << "No measures arguments" << std::endl;
    }

    //std::cout << std::endl;

    return measures;
}


/*
  TODO: BethYw::parseYearsArg(args)

  Parse the years command line argument. Years is either a four digit year 
  value, or two four digit year values separated by a hyphen (i.e. either 
  YYYY or YYYY-ZZZZ).

  This should be parsed as two integers and inserted into a std::tuple,
  representing the start and end year (inclusive). If one or both values are 0,
  then there is no filter to be applied. If no year argument is given return
  <0,0> (i.e. to import all years). You will have to search
  the web for how to construct std::tuple objects! 

  @param args
    Parsed program arguments

  @return
    A std::tuple containing two unsigned ints

  @throws
    std::invalid_argument if the argument contains an invalid years value with
    the message: Invalid input for years argument
*/

YearFilterTuple BethYw::parseYearsArg(cxxopts::ParseResult& args) {
    std::string const ERROR = "Invalid input for years argument";
   std::string temp = args["years"].as<std::string>();
   std::string::size_type split = temp.find('-');
   unsigned int x = std::strtol(temp.c_str(), nullptr, 10);
   unsigned int y = 0;

   //check if the value is in range
   if((x <= 999 || x >= 9999) && x != 0) {
       throw std::invalid_argument(ERROR);
   }

    //if '-' exists
   if(split != std::string::npos) {

       temp = temp.erase(0, split + 1);
       y = std::strtol(temp.c_str(), nullptr, 10); //get the number

       //check if the value is in range
       //x cannot be bigger than y
       if((y < x || y <= 999 || y >= 9999) && y != 0) {
           throw std::invalid_argument(ERROR);
       }
   }
   // std::cout << "The years tuple is " << x << " " << y << std::endl;

    return (YearFilterTuple(x, y));
}

/*
  TODO: BethYw::loadAreas(areas, dir, areasFilter)

  Load the areas.csv file from the directory `dir`. Parse the file and
  create the appropriate Area objects inside the Areas object passed to
  the function in the `areas` argument.

  areas.csv is guaranteed to be formatted as:
    Local authority code,Name (eng),Name (cym)

  Hint: To implement this function. First you will need create an InputFile 
  object with the filename of the areas file, open it, and then pass reference
  to the stream to the Areas::populate() function.

  Hint 2: you can retrieve the specific filename for a dataset, e.g. for the 
  areas.csv file, from the InputFileSource's FILE member variable

  @param areas
    An Areas instance that should be modified (i.e. the populate() function
    in the instance should be called)

  @param dir
    Directory where the areas.csv file is

  @param areasFilter
    An unordered set of areas to filter, or empty to import all areas

  @return
    void

  @example
    Areas areas();

    BethYw::loadAreas(areas, "data", BethYw::parseAreasArg(args));
*/

void BethYw::loadAreas(Areas &areas, std::string const &dir, std::unordered_set<std::string> const &areasFilter) {

    std::string inputString = "../" + dir + InputFiles::AREAS.FILE;
    InputFile file(inputString);
    //we use a pointer because
    //istream cannot be instantiated without a value
    std::istream *stream = file.open().release();

    areas.populate(
            *stream,
            InputFiles::AREAS.PARSER,
            InputFiles::AREAS.COLS,
            &areasFilter);
}

/*
  TODO: BethYw::loadDatasets(areas,
                             dir,
                             datasetsToImport,
                             areasFilter,
                             measuresFilter,
                             yearsFilter)

  Import datasets from `datasetsToImport` as files in `dir` into areas, and
  filtering them with the `areasFilter`, `measuresFilter`, and `yearsFilter`.

  The actual filtering will be done by the Areas::populate() function, thus 
  you need to merely pass pointers on to these flters.

  This function should promise not to throw an exception. If there is an
  error/exception thrown in any function called by thus function, catch it and
  output 'Error importing dataset:', followed by a new line and then the output
  of the what() function on the exception.

  @param areas
    An Areas instance that should be modified (i.e. datasets loaded into it)

  @param dir
    The directory where the datasets are

  @param datasetsToImport
    A vector of InputFileSource objects

  @param areasFilter
    An unordered set of areas (as authority codes encoded in std::strings)
    to filter, or empty to import all areas

  @param measuresFilter
    An unordered set of measures (as measure codes encoded in std::strings)
    to filter, or empty to import all measures

  @param yearsFilter
    An two-pair tuple of unsigned ints corresponding to the range of years 
    to import, which should both be 0 to import all years.

  @return
    void

  @example
    Areas areas();

    BethYw::loadDatasets(
      areas,
      "data",
      BethYw::parseDatasetsArgument(args),
      BethYw::parseAreasArg(args),
      BethYw::parseMeasuresArg(args),
      BethYw::parseYearsArg(args));
*/

void BethYw::loadDatasets(Areas& areas, std::string const &dir,
            std::vector<BethYw::InputFileSource> const &datasetsToImport,
            StringFilterSet const &areasFilter,
            StringFilterSet const &measuresFilter,
            YearFilterTuple const &yearsFilter) {

    for(InputFileSource const &src : datasetsToImport) {

        std::string inputString = "../" + dir + src.FILE;
        InputFile file(inputString);
        std::istream *stream = file.open().release();

        areas.populate(
                *stream,
                src.PARSER,
                src.COLS,
                &areasFilter,
                &measuresFilter,
                &yearsFilter);
    }
}

