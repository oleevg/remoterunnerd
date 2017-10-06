/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef REMOTERUNNERD_APPLICATION_H
#define REMOTERUNNERD_APPLICATION_H

#include <string>
#include <memory>

#include <common/TextConfigurationParser.hpp>
#include <common/CommandStore.hpp>

#include <network/AsyncListener.hpp>

#include "ApplicationService.hpp"

namespace runnerd {

  namespace server {

    class Application {

      public:
        Application(int defaultPort, int defaultTimeout, const std::string& defaultUnixSocket,
                    const std::string& commandsConfigurationFile);

        void parseArguments(int argc, const char** argv);

        int run(int argc, const char** argv);

      private:
        void setArguments();

      private:
        const int defaultPort_;
        const int defaultTimeout_;
        const std::string defaultUnixSocket_;
        const std::string defaultConfiguration_;

        common::TextConfigurationParser::Ptr parser_;
        common::CommandStore::Ptr commandStore_;

        network::AsyncListener::Ptr asyncListener_;

        std::shared_ptr<ApplicationService> appService;

    };

  }

}

#endif //REMOTERUNNERD_APPLICATION_H
