/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef C_TEXTCONFIGURATIONPARSER_HPP
#define C_TEXTCONFIGURATIONPARSER_HPP

#include <string>
#include <vector>
#include <memory>

namespace runnerd {

namespace common {

  typedef std::vector<std::string> ConfigurationContent;

  class TextConfigurationParser {
    public:
      typedef std::shared_ptr<TextConfigurationParser> Ptr;

    public:
      TextConfigurationParser(const std::string& filePath);

      ConfigurationContent readByLine();

    private:
      std::string filePath_;

  };

  }

}


#endif //C_TEXTCONFIGURATIONPARSER_HPP
