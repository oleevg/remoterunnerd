/*
 * ProcessRunnerProtocol.hpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <string>
#include <chrono>
#include <unordered_map>
#include <memory>

#include <boost/core/noncopyable.hpp>

#include <network/IAsyncConnection.hpp>

#include "CommandStore.hpp"

namespace runnerd {

  namespace common {

    /**
     * @brief Describes application's server <-> client interaction protocol.
     */
    class ProcessRunnerProtocol : public std::enable_shared_from_this<ProcessRunnerProtocol>, boost::noncopyable {

      using CommandHandler = std::string (ProcessRunnerProtocol::*)(void);
      using CommandHandlers = std::unordered_map<std::string, CommandHandler>;

    public:
      using Ptr = std::shared_ptr<ProcessRunnerProtocol>;

    public:
      /**
       * @brief ctor.
       * @param connection Connection to communicate with a client.
       * @param commandStore Registered commands storage.
       * @param processExecutionTimeout Acceptable timeout to wait for commands execution.
       */
      ProcessRunnerProtocol(const network::IAsyncConnection::Ptr& connection, const CommandStore::Ptr& commandStore,
                            const std::chrono::milliseconds& processExecutionTimeout);

      /**
       * @brief Starts protocol's steps sequence.
       */
      void start();

      std::chrono::milliseconds getProcessExecutionTimeout() const;

      void close();

    private:
      void registerInternalCommands();
      size_t getReadBufferSize() const;

      const network::IOBuffer& getReadBuffer() const;

      void startReadTaskAsync();
      void startWriteTaskAsync(const std::string& message);

      std::string handleRequest();
      std::string normalizeCommandLine(const network::IOBuffer& commandLine);

      bool isInternalCommand(const std::string& command) const;
      std::string executeInternalCommand(const std::string& command);

      std::string exitHandler();
      std::string listHandler();

    private:
      std::chrono::milliseconds processExecutionTimeout_;

      CommandHandlers commandHandlers_;
      CommandStore::Ptr commandStore_;

      network::IAsyncConnection::Ptr connection_;

      network::IOHandler readHandler_;
      network::IOHandler writeHandler_;
      network::ReadCompleteHandler readCompleteHandler_;

      const std::string commandPrompt_;

      network::IOBuffer readBuffer_;
    };

  } // namespace common

} // namespace runnerd

