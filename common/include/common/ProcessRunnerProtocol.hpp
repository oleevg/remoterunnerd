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

#include <network/AsyncConnection.hpp>

#include "CommandStore.hpp"

namespace runnerd {

  namespace common {

    class ProcessRunnerProtocol : public std::enable_shared_from_this<ProcessRunnerProtocol>, boost::noncopyable {

        typedef std::function<void()> CommandHandler;
        typedef std::unordered_map<std::string, CommandHandler> CommandHandlers;

      public:
        typedef std::shared_ptr<ProcessRunnerProtocol> Ptr;

      public:
        ProcessRunnerProtocol(const network::AsyncConnection::Ptr& connection, const CommandStore::Ptr& commandStore);

        void start();

        const char* getReadBuffer() const;
        void clearReadBuffer();

        void close();

      private:
        void registerInternalCommands();

        void startReadTaskAsync();
        void startWriteTaskAsync(const std::string& message);

      private:
        CommandHandlers commandHandlers_;
        CommandStore::Ptr commandStore_;

        network::AsyncConnection::Ptr connection_;

        network::AsyncConnection::ReadCompleteHandler readCompleteHandler_;
        network::AsyncConnection::IOHandler readHandler_;
        network::AsyncConnection::IOHandler writeHandler_;

        char readBuffer_[UINT16_MAX];

    };

  }

}


#endif //RUNNERD_PROCESSRUNNERPROTOCOL_HPP
