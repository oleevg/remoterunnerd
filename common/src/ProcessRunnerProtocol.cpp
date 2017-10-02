/*
 * ProcessRunnerProtocol.cpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <common/ProcessRunnerProtocol.hpp>

namespace runnerd {

  namespace common {


    ProcessRunnerProtocol::ProcessRunnerProtocol()
    {
      auto exitHandler = [this]() {
        this->close();
      };

      auto listHandler = [this]() {
        ;
      };

      commandHandlers.insert({"exit", exitHandler});
      commandHandlers.insert({"list", listHandler});
    }

    void ProcessRunnerProtocol::start()
    {

    }

    void ProcessRunnerProtocol::sendCommandPrompt()
    {

    }

    std::string ProcessRunnerProtocol::readCommand()
    {
      return std::string();
    }

    void ProcessRunnerProtocol::close()
    {

    }

    void ProcessRunnerProtocol::registerInternalCommands()
    {

    }
  }

}
