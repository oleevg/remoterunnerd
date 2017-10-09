/*
 * ProcessExecutor.cpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <chrono>

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>
#include <core/BaseException.hpp>

#include <common/ProcessExecutor.hpp>

namespace {

  const std::string processExecutionCanceled = "Execution canceled due to the timeout expiration.";

  std::string takeStringFromPipeStream(boost::process::ipstream& stream)
  {
    std::string line;
    std::string output;

    while (std::getline(stream, line))
    {
      output.append(line);
      output.push_back('\n');
    }

    return output;
  }

}

namespace runnerd {

  namespace common {

    std::string
    ProcessExecutor::executeProcess(const std::string& execName, const ProcessExecutor::Arguments& arguments,
                                    int timeout)
    {
      mdebug_info("Going to execute '%s'. ThreadId=0x%x", execName.c_str(), core::ThreadHelper::threadIdToInt());

      mdebug_info("Arguments:");
      for (const auto& arg : arguments)
      {
        mdebug_info("'%s' ", arg.c_str());
      }

      std::string execCommand = execName;
      for (const auto& arg : arguments)
      {
        execCommand.append(" ");
        execCommand.append(arg);
      }

      boost::process::ipstream outputStream;
      boost::process::ipstream errorStream;

      try
      {
        boost::process::child childProcess(execCommand, boost::process::std_out > outputStream,
                                           boost::process::std_err > errorStream);

        std::error_code errorCode;
        if (!childProcess.wait_for(std::chrono::seconds(timeout), errorCode))
        {
          mdebug_warn("Process '%s' execution timeout expired. Going to terminate the child process.", execName.c_str());
          childProcess.terminate();

          return processExecutionCanceled;
        }

        int childErrorCode = childProcess.exit_code();
        if(childErrorCode)
        {
          mdebug_warn("Command '%s' execution failed. Return code: %d.", execName.c_str(), childErrorCode);
          return takeStringFromPipeStream(errorStream);
        }

        return takeStringFromPipeStream(outputStream);
      }
      catch(const boost::process::process_error& exc)
      {
        throw core::BaseException(exc.what());
      }


    }

  }

}
