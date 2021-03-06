


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <979961>

  This file contains the code responsible for opening and closing file
  streams. The actual handling of the data from that stream is handled
  by the functions in data.cpp. See the header file for additional comments.
  
  Each function you must implement has a TODO in its comment block. You may
  have to implement additional functions. You may implement additional
  functions not specified.
 */

#include "input.h"

/*
  TODO: InputSource::InputSource(source)

  Constructor for an InputSource.

  @param source
    A unique identifier for a source (i.e. the location).
*/
InputSource::InputSource(const std::string& source) {
  this->source = source;
}

/*
  TODO: InputSource::getSource()

  This function should be callable from a constant context.

  @return
    A non-modifable value for the source passed into the construtor.
*/

std::string InputSource::getSource() const {
    return source;
}


/*
  TODO: InputFile:InputFile(path)

  Constructor for a file-based source.

  @param path
    The complete path for a file to import.

  @example
    InputFile input("data/areas.csv");
*/
InputFile::InputFile(const std::string& filePath) : InputSource(filePath) {
}

/*
  TODO: InputFile::open()

  Open a file stream to the file path retrievable from getSource()
  and return a reference to the stream.

  @return
    A standard input stream reference

  @throws
    std::runtime_error if there is an issue opening the file, with the message:
    InputFile::open: Failed to open file <file name>

  @example
    InputFile input("data/areas.csv");
    input.open();
*/
std::unique_ptr<std::istream> InputFile::open() {

    auto init_buf = std::make_unique<std::filebuf>();
    //return a unique istream if the file opens, otherwise, throw an exception
    return std::make_unique<std::istream>(
            init_buf->open(getSource(), std::ios_base::in)
            ? init_buf.release()
            : throw std::runtime_error("InputFile::open: Failed to open file " + getSource())
            );
    /*
    std::string source = getSource();


    std::cout<<"trying to open " + source << std::endl;
    std::filebuf fileBuffer;

    if(fileBuffer.open(source, std::ios::in)) {

        std::istream s(&fileBuffer);
        while(!s.eof()) {
            char c = s.get();
            std::cout<< c;
        }

       return &s;
    } else {

    }

    std::ifstream is(source);

    std::string test;
    is >> test;

    if(!is.fail()) {
        std::istream &s = is;

        while(!s.eof()) {
            char c = s.get();
            std::cout<< c;
        }

        return s;
    } else {
        throw new std::runtime_error("InputFile::open: Failed to open file " + source);
    }*/
}
