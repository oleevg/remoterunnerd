/*
 * ProcessRunnerProtocol.cpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <algorithm>

#include<boost/algorithm/string/split.hpp>                                      
#include<boost/algorithm/string.hpp>

#include <common/ProcessRunnerProtocol.hpp>
#include <common/ProcessExecutor.hpp>
#include <network/AsyncConnection.hpp>

namespace runnerd {

  namespace common {


    ProcessRunnerProtocol::ProcessRunnerProtocol(const network::AsyncConnection::Ptr& connection,
                                                     const CommandStore::Ptr& commandStore) :
            connection_(connection), commandStore_(commandStore)
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
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
      };

      writeHandler_ = [selfCopy](const boost::system::error_code &err, size_t bytes)
      {
        selfCopy->startReadTaskAsync();
      };

      readHandler_ = [selfCopy](const boost::system::error_code &err, size_t bytes)
      {
        std::string request(selfCopy->getReadBuffer());
        request.erase(std::remove(request.begin(), request.end(), '\n'), request.end());

        std::string response;

        if(!request.empty())
        {
          ProcessExecutor::Arguments arguments;
        
          boost::algorithm::split(arguments, request, boost::is_any_of("\t "), boost::token_compress_on);

          std::string execName = arguments.front();
          ProcessExecutor::Arguments argumentsWithoutExec;

          if(arguments.size() > 1)
          {
            argumentsWithoutExec.assign(arguments.begin() + 1, arguments.end());
          }

          response = ProcessExecutor::executeProcess(execName, argumentsWithoutExec, 5);
        } 

        response.append("\nrunnerd# ");

        selfCopy->clearReadBuffer();

        selfCopy->startWriteTaskAsync(response);
      };


      // Let's start
      clearReadBuffer();
      connection_->writeAsync("runnerd# ", writeHandler_);
    }

    void ProcessRunnerProtocol::close()
    {

    }

    void ProcessRunnerProtocol::registerInternalCommands()
    {
      auto exitHandler = [this]() {
        this->close();
      };

      auto listHandler = [this]() { ;
      };

      commandHandlers_.insert({"exit", exitHandler});
      commandHandlers_.insert({"list", listHandler});
    }

    void ProcessRunnerProtocol::startReadTaskAsync()
    {
      connection_->readAsync(readBuffer_, sizeof(readBuffer_), readCompleteHandler_, readHandler_);
    }

    void ProcessRunnerProtocol::startWriteTaskAsync(const std::string& message)
    {
      connection_->writeAsync(message, writeHandler_);
    }

    const char* ProcessRunnerProtocol::getReadBuffer() const
    {
      return readBuffer_;
    }

    void ProcessRunnerProtocol::clearReadBuffer()
    {
      memset(readBuffer_, 0, sizeof(readBuffer_));
    }
  }

}
