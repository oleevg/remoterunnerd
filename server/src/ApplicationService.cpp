/*
 * ApplicationService.cpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>

#include <common/ProcessRunnerProtocol.hpp>

#include "ApplicationService.hpp"

namespace runnerd {

  namespace server {

    ApplicationService::ApplicationService(int port, const common::TextConfigurationParser::Ptr& parser,
                                           const common::CommandStore::Ptr& commandStore) :
            parser_(parser), commandStore_(commandStore), asyncListener_(std::make_shared<network::AsyncListener>(port))
    {

    }

    ApplicationService::ApplicationService(const std::string& unixSocketPath,
                                           const common::TextConfigurationParser::Ptr& parser,
                                           const common::CommandStore::Ptr& commandStore) :
            parser_(parser), commandStore_(commandStore)
    {

    }

    int ApplicationService::run()
    {
      auto selfCopy = shared_from_this();

      auto acceptHandler = [selfCopy](const network::AsyncConnection::Ptr& connection, const boost::system::error_code& errorCode){
        mdebug_info("Client connected. ThreadId=%s.\n", core::ThreadHelper::threadIdToString().c_str());
        common::ProcessRunnerProtocol::Ptr protocol = std::make_shared<common::ProcessRunnerProtocol>(connection, selfCopy->commandStore_);
        protocol->start();
      };

      asyncListener_->listenAsync(acceptHandler);

      return 0;
    }

  }

}
