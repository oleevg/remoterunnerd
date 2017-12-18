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
#include <network/IAsyncConnection.hpp>

namespace runnerd {

  namespace common {


    ProcessRunnerProtocol::ProcessRunnerProtocol(const network::IAsyncConnection::Ptr& connection, const CommandStore::Ptr& commandStore,
                                                     int processExecutionTimeout) :
            processExecutionTimeout_(processExecutionTimeout), connection_(connection), commandStore_(commandStore), commandPrompt_("runnerd# ")
    {
      registerInternalCommands();

      readBuffer_.resize(getReadBufferSize());
    }

    void ProcessRunnerProtocol::start()
    {
      auto selfCopy = shared_from_this();

      readCompleteHandler_ = [selfCopy](const boost::system::error_code &err, size_t bytes)
      {
        if (err) return 0;

        auto beginIterator = selfCopy->getReadBuffer().cbegin();
        auto endIterator = selfCopy->getReadBuffer().cbegin();
        std::advance(endIterator, bytes);

        auto iter = std::find(beginIterator, endIterator, '\n');
        bool found = (iter != endIterator);

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
          mdebug_warn("Socket might has been closed. Error code: %d (%s)", err, err.message().c_str());
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
          mdebug_warn("Socket might has been closed. Error code: %d (%s)", err, err.message().c_str());
        }
      };

      // Let's start
      connection_->writeAsync(network::IOBuffer(commandPrompt_.cbegin(), commandPrompt_.cend()),  writeHandler_);
    }

    size_t ProcessRunnerProtocol::getReadBufferSize() const
    {
      static const size_t readBufferSize = 1024;

      return readBufferSize;
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
      connection_->readAsync(readBuffer_, readCompleteHandler_, readHandler_);
    }

    void ProcessRunnerProtocol::startWriteTaskAsync(const std::string& message)
    {
      network::IOBuffer buffer(message.begin(), message.end());
      connection_->writeAsync(buffer, writeHandler_);
    }

    std::string ProcessRunnerProtocol::handleRequest()
    {
      std::string request = normalizeCommandLine(getReadBuffer());

      std::string response;
      if(!request.empty())
      {
        ProcessExecutor::Arguments arguments;
        boost::algorithm::split(arguments, request, boost::is_any_of("\t "), boost::token_compress_on);

        const std::string& execName = arguments.front();

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

    const network::IOBuffer& ProcessRunnerProtocol::getReadBuffer() const
    {
      return readBuffer_;
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

    std::string ProcessRunnerProtocol::normalizeCommandLine(const network::IOBuffer& commandLine)
    {
      // Remove initial spaces
      auto beginIterator = std::find_if_not(commandLine.cbegin(), commandLine.cend(), [](const network::IOBuffer::value_type& value)
      {
        return (value == '\t' || value == ' ');
      });


      // Remove trailing new line symbol
      std::string symbolsEnd("\n\r");
      auto endIterator = std::find_first_of(commandLine.cbegin(), commandLine.cend(), symbolsEnd.cbegin(), symbolsEnd.cend());

      std::string result(beginIterator, endIterator);

      // Test for injections


      return result;
    }
  }

}
