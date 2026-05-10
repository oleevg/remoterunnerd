/*
 * ThreadHelper.hpp
 *
 *  Created on: 10/4/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <sstream>
#include <string>
#include <thread>

namespace runnerd {

  namespace core {

    /**
     * @brief Helper class to work with std::thread identifier.
     */
    class ThreadHelper {
    public:
      static std::string threadIdToString()
      {
        std::stringstream stringStream;
        stringStream << std::this_thread::get_id();

        return stringStream.str();
      }

      static long threadIdToInt()
      {
        std::string strId = threadIdToString();

        return std::stol(strId);
      }
    };

  } // namespace core

} // namespace runnerd

