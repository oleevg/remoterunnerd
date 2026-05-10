/*
 * Application.hpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <memory>

#include <common/TextConfigurationParser.hpp>
#include <common/CommandStore.hpp>

#include <network/AsyncListener.hpp>

#include "ApplicationService.hpp"

namespace runnerd {

  namespace server {

    /**
     * @brief Application main class.
     * @detailed Provides application arguments handling.
     */
    class Application {
      /**
       * @brief Starts application's main cycle.
       */
    public:
      int run(int argc, const char** argv);

    private:
      void parseArguments(int argc, const char** argv);

    private:
      std::shared_ptr<ApplicationService> appService;
    };

  } // namespace server

} // namespace runnerd

