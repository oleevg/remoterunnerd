/*
 * Application.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include <core/ulog.h>
#include <core/SystemException.hpp>
#include <core/ThreadHelper.hpp>

#include "Application.hpp"

namespace {
  void conflicting_options(const boost::program_options::variables_map& vm, const char* rOpt, const char* lOpt)
  {
    if (vm.count(lOpt) && !vm[lOpt].defaulted() && vm.count(rOpt) && !vm[rOpt].defaulted())
    {
      throw runnerd::core::BaseException((boost::format("You can't set options '%s' and '%s' at the same time. Choose only one of them to continue.") % lOpt % rOpt).str());
    }
  }
}

namespace runnerd {

  namespace server {

    void Application::parseArguments(int argc, const char** argv)
    {
      namespace options = boost::program_options;

      /* Default parameters definition */
      const int portDefault = 12345;
      const int timeoutDefault = 15;
      const size_t threadPoolSizeDefault = 5;
      const std::string unixSocketDefault = "/tmp/simple-telnetd";
      const std::string commandsConfigurationFileDefault = "/etc/remote-runnerd.conf";
      const bool useUnixSocketDefault = false;

      options::options_description optionDescription((boost::format("Usage: %s [options]... \nOptions") % argv[0]).str());

      int port = portDefault;
      int timeout = timeoutDefault;
      size_t threadPoolSize = threadPoolSizeDefault;
      std::string configurationFile = commandsConfigurationFileDefault;
      std::string unixSocket = unixSocketDefault;
      bool useUnixSocket = useUnixSocketDefault;
      bool debugRun = false;

      optionDescription.add_options()
              ("port,p", options::value<int>(&port)->default_value(portDefault), "The port number to listen.")
              ("config,c", options::value<std::string>(&configurationFile)->default_value(commandsConfigurationFileDefault),
               "The configuration file with supported commands list.")
              ("socket,s", options::value<std::string>(&unixSocket)->default_value(unixSocketDefault), "The Unix socket path.")
              ("timeout,t", options::value<int>(&timeout)->default_value(timeoutDefault), "Process execution wait timeout in seconds.")
              ("threads", options::value<size_t>(&threadPoolSize)->default_value(threadPoolSizeDefault), "The service's thread pool size.")
              ("unix,u", options::bool_switch(&useUnixSocket)->default_value(useUnixSocketDefault), "Force to use Unix socket.")
              ("debug,d", options::bool_switch(&debugRun)->default_value(false), "Interactive run without daemonizing.")
              ("help,h", "As it says.");

      options::variables_map variableMap;

      options::store(options::parse_command_line(argc, argv, optionDescription), variableMap);
      options::notify(variableMap);

      if(variableMap.count("help"))
      {
        std::cout << optionDescription << "\n";
        exit(0);
      }

      conflicting_options(variableMap, "port", "socket");
      conflicting_options(variableMap, "port", "unix");

      if(timeout <= 0)
      {
        throw core::BaseException("Negative or zero timeout values are not supported. Please provide some positive integer value.");
      }

      if(threadPoolSize < 1)
      {
        throw core::BaseException("The number of service threads can't be less than one. Please provide some positive integer value.");
      }

      common::TextConfigurationParser::Ptr parser = std::make_shared<common::TextConfigurationParser>(
              configurationFile);

      auto content = parser->readByLine();

      common::CommandStore::Ptr commandStore = std::make_shared<common::CommandStore>(content.size());
      commandStore->setAllCommands(content);

      if(useUnixSocket || !variableMap["socket"].defaulted())
      {
        // TODO: Temporary hack. Need to implement proper exclusive application run.
        remove(unixSocket.c_str());
        appService = std::make_shared<ApplicationService>(unixSocket, timeout, threadPoolSize, parser, commandStore, !debugRun);
      }
      else
      {
        appService = std::make_shared<ApplicationService>(port, timeout, threadPoolSize, parser, commandStore, !debugRun);
      }

    }

    int Application::run(int argc, const char** argv)
    {
      int rc = 0;

      try
      {
        parseArguments(argc, argv);
        rc = appService->run();
      }
      catch (const core::SystemException& exc)
      {
        mdebug_error(exc.what());
        rc = exc.getErrorCode();
      }
      catch (const core::BaseException& exc)
      {
        mdebug_error(exc.what());
        rc = -1;
      }
      catch(const std::exception& exc)
      {
        mdebug_error("Unknown error occurred. %s", exc.what());
        rc = -1;
      }

      return rc;
    }

  }

}
