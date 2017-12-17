/*
 * ApplicationService.hpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_APPLICATIONSERVICE_HPP
#define RUNNERD_APPLICATIONSERVICE_HPP

#include <memory>
#include <string>

#include <boost/core/noncopyable.hpp>

#include <common/TextConfigurationParser.hpp>
#include <common/CommandStore.hpp>
#include <common/UnixService.hpp>

#include <network/IAsyncListener.hpp>

namespace runnerd {

  namespace server {

    /**
     * @brief Service class supporting signals and running as a daemon.
     */
    class ApplicationService : public std::enable_shared_from_this<ApplicationService>, boost::noncopyable {

      public:
        typedef std::shared_ptr<ApplicationService> Ptr;

      public:
        /**
         * @brief ctor.
         * @param port Port number to listen to.
         * @param processExecutionTimeout Acceptable timeout to wait for a process execution.
         * @param parser Application's configuration file parser.
         * @param commandStore Registered commands storage.
         * @param daemonize Whether run as a daemon or not.
         */
        ApplicationService(int port, int processExecutionTimeout, size_t threadPoolSize,
                           const common::TextConfigurationParser::Ptr& parser, const common::CommandStore::Ptr& commandStore,
                           bool daemonize);

        /**
         * @brief ctor.
         * @param unixSocketPath Path to the local socket.
         * @param processExecutionTimeout Acceptable timeout to wait for a process execution.
         * @param parser Application's configuration file parser.
         * @param commandStore Registered commands storage.
         * @param daemonize Whether run as a daemon or not.
         */
        ApplicationService(const std::string& unixSocketPath, int processExecutionTimeout, size_t threadPoolSize,
                           const common::TextConfigurationParser::Ptr& parser, const common::CommandStore::Ptr& commandStore,
                           bool daemonize);
        /**
         * @return Flag saying whether application runs as a daemon or not.
         */
        bool isDaemonized() const;

        /**
         * @brief Starts the application main cycle.
         * @return Execution exit code.
         */
        int run();

      private:
        void initialize();

        void signalHandlerTask();

      private:
        common::TextConfigurationParser::Ptr parser_;
        common::CommandStore::Ptr commandStore_;

        network::IAsyncListener::Ptr asyncListener_;

        common::UnixService::SignalHandlerFlag::Ptr termFlag_;
        common::UnixService::SignalHandlerFlag::Ptr intFlag_;
        common::UnixService::SignalHandlerFlag::Ptr hupFlag_;

        int processExecutionTimeout_;
        bool daemonized_;
    };

  }

}


#endif //RUNNERD_APPLICATIONSERVICE_HPP
