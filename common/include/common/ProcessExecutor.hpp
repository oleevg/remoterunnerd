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

    /**
     * @brief Executes commands and provides access to its output.
     */
    class ProcessExecutor {

      public:
        typedef std::vector<std::string> Arguments;

      public:
        /**
         *
         * @param execName Command name to execute.
         * @param arguments The collection of command arguments.
         * @param timeout Acceptable timeout in ms to wait for command execution.
         * @return Command's output.
         */
        std::string executeProcess(const std::string& execName, const Arguments& arguments, int timeout);

    };

  }

}



#endif //RUNNERD_PROCESSEXECUTOR_HPP
