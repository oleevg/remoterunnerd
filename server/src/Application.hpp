/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef REMOTERUNNERD_APPLICATION_H
#define REMOTERUNNERD_APPLICATION_H

#include <string>

#include <common/TextConfigurationParser.hpp>
#include <common/CommandStore.hpp>

namespace runnerd
{

  namespace server {

    class Application {

      public:
        Application(int defaultPort, int defaultTimeout, const std::string& defaultUnixSocket,
                    const std::string& commandsConfigurationFile);

        void parseArguments(int argc, const char** argv);

        int runInteractive();

        int runForeground();

      private:
        void setArguments();

      private:
        const int defaultPort_;
        const int defaultTimeout_;
        const std::string defaultUnixSocket_;
        const std::string defaultConfiguration_;

        common::TextConfigurationParser::Ptr parser;
        common::CommandStore::Ptr commandStore;
    };

  }

}

#endif //REMOTERUNNERD_APPLICATION_H
