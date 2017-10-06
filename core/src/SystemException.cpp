/*
 * SystemException.cpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/SystemException.hpp>

namespace runnerd {

  namespace core {

    SystemException::SystemException(const std::string& errorMessage, int errorCode) : BaseException(errorMessage),
                                                                                       errorCode_(errorCode)
    {

    }

    int SystemException::getErrorCode() const
    {
      return errorCode_;
    }

  }

}


