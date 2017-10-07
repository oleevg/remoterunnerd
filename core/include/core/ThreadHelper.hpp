/*
 * ThreadHelper.hpp
 *
 *  Created on: 10/4/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_THREADMACRO_HPP_HPP
#define RUNNERD_THREADMACRO_HPP_HPP

#include <sstream>
#include <string>
#include <thread>

namespace runnerd {

  namespace core {

    class ThreadHelper {
      public:
        static std::string threadIdToString()
        {
          std::stringstream stringStream;
          stringStream << std::this_thread::get_id();

          return stringStream.str();
        }

        static size_t threadIdToInt()
        {
          std::string strId = threadIdToString();

          return std::stol(strId);
        }
    };

  }

}


#endif //RUNNERD_THREADMACRO_HPP_HPP
