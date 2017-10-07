/*
 * ApplicationService.cpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <signal.h>

#include <memory>
#include <thread>
#include <chrono>
#include <future>

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>

#include <common/ProcessRunnerProtocol.hpp>

#include "ApplicationService.hpp"

namespace runnerd {

  namespace server {

    ApplicationService::ApplicationService(int port, const common::TextConfigurationParser::Ptr& parser,
                                               const common::CommandStore::Ptr& commandStore, bool daemonize) :
            parser_(parser), commandStore_(commandStore), asyncListener_(std::make_shared<network::AsyncListener>(port)), daemonized_(daemonize)
    {
      initialize();
    }

    ApplicationService::ApplicationService(const std::string& unixSocketPath, const common::TextConfigurationParser::Ptr& parser,
                                               const common::CommandStore::Ptr& commandStore, bool daemonize) :
            parser_(parser), commandStore_(commandStore), daemonized_(daemonize)
    {
      initialize();
    }

    int ApplicationService::run()
    {
      if(isDaemonized())
      {
        unixService_->daemonize();
      }

      unixService_->registerSignalHandlerFlag(SIGTERM, termFlag_);
      unixService_->registerSignalHandlerFlag(SIGHUP, hupFlag_);

      unixService_->setSignalHandler();

      std::future<void> signalHandlerTask = std::async(std::launch::async, &ApplicationService::signalHandlerTask, this);

      auto selfCopy = shared_from_this();

      auto acceptHandler = [selfCopy](const network::AsyncConnection::Ptr& connection, const boost::system::error_code& errorCode){
        mdebug_info("Client connected. ThreadId=0x%x.\n", core::ThreadHelper::threadIdToInt());
        common::ProcessRunnerProtocol::Ptr protocol = std::make_shared<common::ProcessRunnerProtocol>(connection, selfCopy->commandStore_, 15);
        protocol->start();
      };

      asyncListener_->listenAsync(acceptHandler);

      signalHandlerTask.wait();

      return 0;
    }

    bool ApplicationService::isDaemonized() const
    {
      return daemonized_;
    }

    void ApplicationService::initialize()
    {
      termFlag_ = std::make_shared<common::UnixService::SignalHandlerFlag>(false);
      hupFlag_ = std::make_shared<common::UnixService::SignalHandlerFlag>(false);

      unixService_ = std::unique_ptr<common::UnixService>(new common::UnixService());
    }

    void ApplicationService::signalHandlerTask()
    {
      mdebug_info("%s started", __PRETTY_FUNCTION__);

      while (true)
      {
        if (termFlag_->flag.load())
        {
          mdebug_info("SIGTERM received.");
          termFlag_->flag.store(false);

          asyncListener_->stop();

          return;
        }

        if (hupFlag_->flag.load())
        {
          hupFlag_->flag.store(false);

          mdebug_info("SIGHUP received.");

          auto content = parser_->readByLine();
          commandStore_->setAllCommands(content);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }

  }

}
