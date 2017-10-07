/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>

#include "Application.hpp"

namespace runnerd {

  namespace server {

    Application::Application(int defaultPort, int defaultTimeout, const std::string& defaultUnixSocket,
                             const std::string& commandsConfigurationFile) :
            defaultPort_(defaultPort), defaultTimeout_(defaultTimeout), defaultUnixSocket_(defaultUnixSocket),
            defaultConfiguration_(commandsConfigurationFile)
    {

    }


    void Application::parseArguments(int argc, const char** argv)
    {
      namespace options = boost::program_options;

      int port = defaultPort_;
      int timeout = defaultTimeout_;
      std::string configurationFile = defaultConfiguration_;
      std::string unixSocket = defaultUnixSocket_;

      options::options_description desc((boost::format("Usage: %s [options]... \nOptions") % argv[0]).str());

      desc.add_options()
              ("port,p", options::value<int>(&port)->default_value(defaultPort_), "The port number to listen.")
              ("config,c", options::value<std::string>(&configurationFile),
               "The configuration file with supported commands list.")
              ("socket,s", options::value<std::string>(&unixSocket), "The UNIX socket path.")
              ("timeout,t", options::value<int>(&timeout), "Process execution wait time in ms.")
              ("help,h", "As it says.");

      // Variable to store our command line arguments.
      options::variables_map vm;

      // Parsing and storing arguments.
      options::store(options::parse_command_line(argc, argv, desc), vm);

      // Must be called after all the parsing and storing.
      options::notify(vm);

      if(vm.count("help"))
      {
        std::cout << desc << "\n";
      }

      common::TextConfigurationParser::Ptr parser = std::make_shared<common::TextConfigurationParser>(configurationFile);

      auto content = parser->readByLine();

      common::CommandStore::Ptr commandStore = std::make_shared<common::CommandStore>(content.size());
      commandStore->setAllCommands(content);

      appService = std::make_shared<ApplicationService>(vm["port"].as<int>(), parser, commandStore);
    }

    void Application::setArguments()
    {

    }

    int Application::run(int argc, const char** argv)
    {
      parseArguments(argc, argv);

      return appService->run();
    }

  }

}
