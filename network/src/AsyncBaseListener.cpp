/*
 * AsyncBaseListener.cpp
 *
 *  Created on: 10/8/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>

#include <network/AsyncBaseListener.hpp>

namespace runnerd {

  namespace network {

    AsyncBaseListener::AsyncBaseListener(size_t threadPoolSize):
    threadPoolSize_(threadPoolSize)
    {

    }

    void AsyncBaseListener::stop()
    {
      service_.stop();
    }

    void AsyncBaseListener::listenAsync(AcceptHandler asyncHandler)
    {
      acceptAsync(asyncHandler);

      mdebug_info("Listener started.\n");
      for(size_t i = 0; i<threadPoolSize_; ++i)
      {
        threadPool_.emplace_back([this]() {
          getIoService().run();
        });
      }
    }

    boost::asio::io_service& AsyncBaseListener::getIoService()
    {
      return service_;
    }

    void AsyncBaseListener::wait()
    {
      for (auto& serviceThread : threadPool_)
      {
        serviceThread.join();
      }

      mdebug_info("Listener finished.\n");
    }

  }

}
