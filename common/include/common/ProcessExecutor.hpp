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
#include <chrono>
#include <memory>

namespace runnerd {

  namespace common {

    class ProcessExecutorImpl;

    /**
     * @brief Executes commands and provides access to its output.
     */
    class ProcessExecutor {
      public:
        typedef std::vector<std::string> Arguments;

        struct Messages
        {
          static const char* Canceled;
        };

      public:
        ProcessExecutor();

        ~ProcessExecutor();

        /**
         * @brief Executes provided command with the specified arguments.
         * @param execName Command name to execute.
         * @param arguments The collection of command arguments.
         * @param timeout Acceptable timeout in seconds to wait for command execution.
         * @return Command's output.
         */
        std::string executeProcess(const std::string& execName, const Arguments& arguments, const std::chrono::milliseconds& timeout);

      private:
        std::unique_ptr<ProcessExecutorImpl> impl;
    };

  }

}



#endif //RUNNERD_PROCESSEXECUTOR_HPP
