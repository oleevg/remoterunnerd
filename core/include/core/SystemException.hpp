/*
 * SystemException.hpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_SYSTEMEXCEPTION_HPP
#define RUNNERD_SYSTEMEXCEPTION_HPP

#include "BaseException.hpp"

namespace runnerd {

  namespace core {

    class SystemException : public BaseException {

      public:
        SystemException(const std::string& errorMessage, int errorCode);

        int getErrorCode() const;

      private:
        int errorCode_;
    };

  }

}


#endif //RUNNERD_SYSTEMEXCEPTION_HPP
