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

namespace runnerd {

  namespace common {

    class ProcessRunnerProtocol {

        typedef std::function<void()> CommandHandler;
        typedef std::unordered_map<std::string, CommandHandler> CommandHandlers;

      public:
        ProcessRunnerProtocol();

        void start();

        void sendCommandPrompt();

        std::string readCommand();

        void close();

      private:
        void registerInternalCommands();

      private:
        CommandHandlers commandHandlers;
    };

  }

}


#endif //RUNNERD_PROCESSRUNNERPROTOCOL_HPP
