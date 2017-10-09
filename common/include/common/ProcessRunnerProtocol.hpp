/*
 * ProcessRunnerProtocol.hpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_PROCESSRUNNERPROTOCOL_HPP
#define RUNNERD_PROCESSRUNNERPROTOCOL_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>

#include <network/IAsyncConnection.hpp>

#include "CommandStore.hpp"

namespace runnerd {

  namespace common {

    /**
     * @brief Represents application's server <-> client interaction protocol.
     */
    class ProcessRunnerProtocol : public std::enable_shared_from_this<ProcessRunnerProtocol>, boost::noncopyable {

        typedef std::string (ProcessRunnerProtocol::*CommandHandler)(void);
        typedef std::unordered_map<std::string, CommandHandler> CommandHandlers;

      public:
        typedef std::shared_ptr<ProcessRunnerProtocol> Ptr;

      public:
        /**
         * @brief ctor.
         * @param connection Connection to communicate with a client.
         * @param commandStore Registered commands storage.
         * @param processExecutionTimeout Acceptable timeout to wait for commands execution.
         */
        ProcessRunnerProtocol(const network::IAsyncConnection::Ptr& connection, const CommandStore::Ptr& commandStore,
                                      int processExecutionTimeout = -1);

        /**
         * @brief Starts protocol's steps sequence.
         */
        void start();

        int getProcessExecutionTimeout() const;
        const char* getReadBuffer() const;
        void clearReadBuffer();

        void close();

      private:
        void registerInternalCommands();

        void startReadTaskAsync();
        void startWriteTaskAsync(const std::string& message);

        std::string handleRequest();
        std::string normalizeCommandLine(const std::string& commandLine);

        bool isInternalCommand(const std::string& command) const;
        std::string executeInternalCommand(const std::string& command);

        std::string exitHandler();
        std::string listHandler();

      private:
        int processExecutionTimeout_;

        CommandHandlers commandHandlers_;
        CommandStore::Ptr commandStore_;

        network::IAsyncConnection::Ptr connection_;

        network::IOHandler readHandler_;
        network::IOHandler writeHandler_;
        network::ReadCompleteHandler readCompleteHandler_;

        char readBuffer_[UINT16_MAX];

    };

  }

}


#endif //RUNNERD_PROCESSRUNNERPROTOCOL_HPP
