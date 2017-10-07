/*
 * ProcessExecutor.hpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_PROCESSEXECUTOR_HPP
#define RUNNERD_PROCESSEXECUTOR_HPP

#include <string>
#include <vector>

namespace runnerd {

  namespace common {

    class ProcessExecutor {

      public:
        typedef std::vector<std::string> Arguments;

      public:
        std::string executeProcess(const std::string& execName, const Arguments& arguments, int timeout);

    };

  }

}



#endif //RUNNERD_PROCESSEXECUTOR_HPP
