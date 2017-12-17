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
#include <network/AsyncListener.hpp>
#include <network/AsyncLocalListener.hpp>

#include "ApplicationService.hpp"

namespace runnerd {

  namespace server {

    ApplicationService::ApplicationService(int port, int processExecutionTimeout, size_t threadPoolSize,
                                               const common::TextConfigurationParser::Ptr& parser, const common::CommandStore::Ptr& commandStore,
                                               bool daemonize) :
            parser_(parser), commandStore_(commandStore),
            asyncListener_(std::make_shared<network::AsyncListener>(port, threadPoolSize)),
            processExecutionTimeout_(processExecutionTimeout), daemonized_(daemonize)
    {
      initialize();
    }

    ApplicationService::ApplicationService(const std::string& unixSocketPath, int processExecutionTimeout, size_t threadPoolSize,
                                               const common::TextConfigurationParser::Ptr& parser, const common::CommandStore::Ptr& commandStore,
                                               bool daemonize)
            :
            parser_(parser), commandStore_(commandStore),
            asyncListener_(std::make_shared<network::AsyncLocalListener>(unixSocketPath, threadPoolSize)),
            processExecutionTimeout_(processExecutionTimeout), daemonized_(daemonize)
    {
      initialize();
    }

    int ApplicationService::run()
    {
      common::UnixService& unixService = common::UnixService::get_mutable_instance();

      if(isDaemonized())
      {
        unixService.daemonize();
      }

      unixService.registerSignalHandlerFlag(SIGTERM, termFlag_);
      unixService.registerSignalHandlerFlag(SIGINT, intFlag_);
      unixService.registerSignalHandlerFlag(SIGHUP, hupFlag_);

      unixService.setSignalHandler();

      std::future<void> signalHandlerTask = std::async(std::launch::async, &ApplicationService::signalHandlerTask, this);

      auto selfCopy = shared_from_this();

      auto acceptHandler = [selfCopy](const network::IAsyncConnection::Ptr& connection, const boost::system::error_code& errorCode){
        mdebug_info("Client connected. ThreadId=0x%x.\n", core::ThreadHelper::threadIdToInt());
        common::ProcessRunnerProtocol::Ptr protocol = std::make_shared<common::ProcessRunnerProtocol>(connection, selfCopy->commandStore_, selfCopy->processExecutionTimeout_);
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
      intFlag_ = std::make_shared<common::UnixService::SignalHandlerFlag>(false);
      hupFlag_ = std::make_shared<common::UnixService::SignalHandlerFlag>(false);
    }

    void ApplicationService::signalHandlerTask()
    {
      common::UnixService& unixService = common::UnixService::get_mutable_instance();

      while (true)
      {
        unixService.waitForSignalsSync();

        if (termFlag_->flag.load())
        {
          mdebug_info("Signal SIGTERM received.");
          termFlag_->flag.store(false);

          asyncListener_->stop();

          return;
        }

        if (intFlag_->flag.load())
        {
          mdebug_info("Signal SIGINT received.");
          intFlag_->flag.store(false);

          asyncListener_->stop();

          return;
        }

        if (hupFlag_->flag.load())
        {
          hupFlag_->flag.store(false);

          mdebug_info("Signal SIGHUP received.");

          auto content = parser_->readByLine();
          commandStore_->setAllCommands(content);
        }
      }
    }

  }

}
