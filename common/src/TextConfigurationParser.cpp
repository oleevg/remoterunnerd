/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <fstream>
#include <string>

#include <boost/format.hpp>

#include <core/BaseException.hpp>

#include <common/TextConfigurationParser.hpp>

namespace runnerd {

  namespace common {


    TextConfigurationParser::TextConfigurationParser(const std::string& filePath):
    filePath_(filePath)
    {

    }

    ConfigurationContent TextConfigurationParser::readByLine()
    {
      ConfigurationContent result;

      std::ifstream file(filePath_, std::ios_base::in);
      if(!file.is_open())
      {
        throw core::BaseException((boost::format("Couldn't open the file '%s' for reading. The file might be missing or you don't have enough permissions to read it.") % filePath_.c_str()).str());
      }

      std::string line;
      while (getline(file, line))
      {
        result.push_back(line);
      }

      return result;
    }
  }

}
