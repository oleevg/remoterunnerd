/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>

#include "Application.hpp"

namespace runnerd {

  class ULogWrapper {
    public:
      ULogWrapper()
      {
        ulog_init();
      }

      ~ULogWrapper()
      {
        ulog_clean();
      }
  };

}

int main(int argc, const char* argv[])
{
  const int portDefault = 12345;
  const int timeoutDefault = 5;
  const std::string unixSocketDefault = "/tmp/simple-telnetd";
  //const std::string commandsConfigurationFile = "/etc/remote-runnerd.conf";
  const std::string commandsConfigurationFile = "/home/lolo/remote-runnerd.conf";

  runnerd::ULogWrapper ulog;

  runnerd::server::Application app(portDefault, timeoutDefault, unixSocketDefault, commandsConfigurationFile);

  app.parseArguments(argc, argv);

  return app.runInteractive();
}

