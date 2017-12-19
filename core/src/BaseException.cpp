/*
 * BaseException.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/BaseException.hpp>

namespace runnerd {

  namespace core {

    BaseException::BaseException(const std::string& errorMessage) :
    errorMessage_("Base error occured: " + errorMessage)
    {

    }

    const char* BaseException::what() const noexcept
    {
      return errorMessage_.c_str();
    }
  }

}

