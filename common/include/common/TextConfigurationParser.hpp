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

  /**
   * @brief Handles simple text files.
   */
  class TextConfigurationParser {
    public:
      typedef std::shared_ptr<TextConfigurationParser> Ptr;

    public:
      /**
       * @brief ctor.
       * @param filePath The file path to work with.
       */
      TextConfigurationParser(const std::string& filePath);

      /**
       * @brief Reads the file by lines.
       * @return Custom collection of strings the file contains.
       */
      ConfigurationContent readByLine();

    private:
      std::string filePath_;

  };

  }

}


#endif //C_TEXTCONFIGURATIONPARSER_HPP
