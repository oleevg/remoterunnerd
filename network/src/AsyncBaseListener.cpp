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

      for (auto& serviceThread : threadPool_)
      {
        serviceThread.join();
      }
    }

    void AsyncBaseListener::listenAsync(AcceptHandler asyncHandler)
    {
      acceptAsync(asyncHandler);

      mdebug_info("Listener started.\n");
      for(size_t i = 0; i<threadPoolSize_; ++i)
      {
        threadPool_.push_back(std::thread([this]() {
          getIoService().run();
        }));
      }
      mdebug_info("Listener finished.\n");
    }

    boost::asio::io_service& AsyncBaseListener::getIoService()
    {
      return service_;
    }

  }

}
