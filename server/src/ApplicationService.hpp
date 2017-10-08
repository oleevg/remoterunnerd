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

//
//namespace runnerd {
//  namespace common {
//
//    class UnixService;
//    struct UnixService::SignalHandlerFlag;
//    class UnixService::SignalHandlerFlag::Ptr;
//
//  }
//}

namespace runnerd {

  namespace server {

    class ApplicationService : public std::enable_shared_from_this<ApplicationService>, boost::noncopyable {

      public:
        typedef std::shared_ptr<ApplicationService> Ptr;

      public:
        ApplicationService(int port, const common::TextConfigurationParser::Ptr& parser,
                                   const common::CommandStore::Ptr& commandStore, bool daemonize = false);

        ApplicationService(const std::string& unixSocketPath, const common::TextConfigurationParser::Ptr& parser,
                                   const common::CommandStore::Ptr& commandStore, bool daemonize = false);

        bool isDaemonized() const;

        int run();

      private:
        void initialize();

        void signalHandlerTask();

      private:
        common::TextConfigurationParser::Ptr parser_;
        common::CommandStore::Ptr commandStore_;

        network::IAsyncListener::Ptr asyncListener_;
        std::unique_ptr<common::UnixService> unixService_;

        common::UnixService::SignalHandlerFlag::Ptr termFlag_;
        common::UnixService::SignalHandlerFlag::Ptr hupFlag_;

        bool daemonized_;
    };

  }

}


#endif //RUNNERD_APPLICATIONSERVICE_HPP
