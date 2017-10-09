/*
 * ProcessRunnerProtocol.cpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <algorithm>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>
#include <core/BaseException.hpp>

#include <common/ProcessRunnerProtocol.hpp>
#include <common/ProcessExecutor.hpp>
#include <network/AsyncConnection.hpp>

namespace runnerd {

  namespace common {


    ProcessRunnerProtocol::ProcessRunnerProtocol(const network::IAsyncConnection::Ptr& connection, const CommandStore::Ptr& commandStore,
                                                     int processExecutionTimeout) :
            processExecutionTimeout_(processExecutionTimeout), connection_(connection), commandStore_(commandStore), commandPrompt_("runnerd# ")
    {
      registerInternalCommands();
    }

    void ProcessRunnerProtocol::start()
    {
      auto selfCopy = shared_from_this();

      readCompleteHandler_ = [selfCopy](const boost::system::error_code &err, size_t bytes)
      {
        if (err) return 0;

        bool found = std::find(selfCopy->getReadBuffer(), selfCopy->getReadBuffer() + bytes, '\n') < selfCopy->getReadBuffer() + bytes;
        return found ? 0 : 1;
      };

      writeHandler_ = [selfCopy](const boost::system::error_code &err, size_t bytes)
      {
        if(!err)
        {
          mdebug_info("Response has been sent. ThreadId=0x%x.\n", core::ThreadHelper::threadIdToInt());
          selfCopy->startReadTaskAsync();
        }
        else
        {
          mdebug_warn("Socket might has been closed. Error code: %d", err);
        }
      };

      readHandler_ = [selfCopy](const boost::system::error_code &err, size_t bytes)
      {
        if(!err)
        {
          mdebug_info("Request has been received. ThreadId=0x%x.\n", core::ThreadHelper::threadIdToInt());
          std::string response = selfCopy->handleRequest();
          selfCopy->startWriteTaskAsync(response);
        }
        else
        {
          mdebug_warn("Socket might has been closed. Error code: %d", err);
        }
      };

      // Let's start
      clearReadBuffer();
      connection_->writeAsync(commandPrompt_, writeHandler_);
    }

    void ProcessRunnerProtocol::close()
    {
      connection_->close();
    }

    void ProcessRunnerProtocol::registerInternalCommands()
    {
      commandHandlers_.insert({"exit", &ProcessRunnerProtocol::exitHandler});
      commandHandlers_.insert({"list", &ProcessRunnerProtocol::listHandler});
    }

    void ProcessRunnerProtocol::startReadTaskAsync()
    {
      connection_->readAsync(readBuffer_, sizeof(readBuffer_), readCompleteHandler_, readHandler_);
    }

    void ProcessRunnerProtocol::startWriteTaskAsync(const std::string& message)
    {
      connection_->writeAsync(message, writeHandler_);
    }

    std::string ProcessRunnerProtocol::handleRequest()
    {
      std::string temp(getReadBuffer());

      std::string request = normalizeCommandLine(getReadBuffer());

      clearReadBuffer();

      std::string response;
      if(!request.empty())
      {
        ProcessExecutor::Arguments arguments;

        boost::algorithm::split(arguments, request, boost::is_any_of("\t "), boost::token_compress_on);

        std::string execName = arguments.front();

        if(isInternalCommand(execName))
        {
          response = executeInternalCommand(execName);
        }
        else if(commandStore_->isRegistered(execName))
        {
          ProcessExecutor::Arguments argumentsWithoutExec;

          if (arguments.size() > 1)
          {
            argumentsWithoutExec.assign(arguments.begin() + 1, arguments.end());
          }

          try
          {
            response = ProcessExecutor().executeProcess(execName, argumentsWithoutExec, getProcessExecutionTimeout());
          }
          catch (const core::BaseException& exc)
          {
            response = exc.what();
          }
        }
        else
        {
          mdebug_warn("Attempt to execute unregistered command: '%s'", execName.c_str());
          response = (boost::format("Command '%s' is not registered. You can't execute unregistered commands.") % execName.c_str()).str();
        }
      }

      response.append("\n");
      response.append(commandPrompt_);

      return response;
    }

    const char* ProcessRunnerProtocol::getReadBuffer() const
    {
      return readBuffer_;
    }

    void ProcessRunnerProtocol::clearReadBuffer()
    {
      memset(readBuffer_, 0, sizeof(readBuffer_));
    }

    int ProcessRunnerProtocol::getProcessExecutionTimeout() const
    {
      return processExecutionTimeout_;
    }

    bool ProcessRunnerProtocol::isInternalCommand(const std::string& command) const
    {
      return commandHandlers_.count(command);
    }

    std::string ProcessRunnerProtocol::executeInternalCommand(const std::string& command)
    {
      return (this->*commandHandlers_.at(command))();
    }

    std::string ProcessRunnerProtocol::exitHandler()
    {
      close();

      return std::string("Going to exit...");
    }

    std::string ProcessRunnerProtocol::listHandler()
    {
      std::string response;

      for (const auto& command: commandStore_->getAllCommands())
      {
        response.append(command);
        response.push_back('\n');
      }

      for (const auto& command: commandHandlers_)
      {
        response.append(command.first);
        response.push_back('\n');
      }

      return response;
    }

    std::string ProcessRunnerProtocol::normalizeCommandLine(const std::string& commandLine)
    {
      std::string result = commandLine;

      // Remove initial spaces
      auto pos = commandLine.find_first_not_of("\t ");
      if(pos != std::string::npos && pos != 0)
      {
        result.assign(getReadBuffer() + pos);
      }

      // Remove trailing new line symbol
      pos = result.find_first_of("\n\r");
      if(pos != std::string::npos)
      {
        result.erase(pos);
      }

      // Test for injections


      return result;
    }
  }

}
