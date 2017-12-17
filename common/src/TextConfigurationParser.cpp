/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <fstream>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

      if(access(filePath_.c_str(), R_OK))
      {
        throw core::BaseException((boost::format("Couldn't open the file '%s' for reading: %s") % filePath_.c_str() % strerror(errno)).str());
      }

      std::ifstream file(filePath_, std::ios_base::in);
      if(!file.is_open())
      {
        throw core::BaseException((boost::format("Couldn't open the file '%s': Unknown error occurred.") % filePath_.c_str()).str());
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
