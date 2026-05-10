/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace runnerd {

  namespace common {

    using ConfigurationContent = std::vector<std::string>;

    /**
     * @brief Provides reading of simple text configuration files.
     */
    class TextConfigurationParser {
    public:
      using Ptr = std::shared_ptr<TextConfigurationParser>;

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

  } // namespace common

} // namespace runnerd
