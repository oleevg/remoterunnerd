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

#include <network/AsyncListener.hpp>


// TODO: try to use forward declaration
//namespace runnerd {
//  namespace common {
//
//    class UnixService;
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
                                   const common::CommandStore::Ptr& commandStore);

        ApplicationService(const std::string& unixSocketPath, const common::TextConfigurationParser::Ptr& parser,
                                   const common::CommandStore::Ptr& commandStore);

        int run();

      private:
        common::TextConfigurationParser::Ptr parser_;
        common::CommandStore::Ptr commandStore_;

        network::AsyncListener::Ptr asyncListener_;
        std::unique_ptr<common::UnixService> unixService_;
    };

  }

}


#endif //RUNNERD_APPLICATIONSERVICE_HPP
