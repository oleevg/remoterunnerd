/*
 * Listener.cpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <memory>
#include <thread>

#include <core/ulog.h>

#include <network/AsyncListener.hpp>
#include <core/ThreadHelper.hpp>

namespace runnerd {

  namespace network {

    AsyncListener::AsyncListener(int port):
            acceptor_(service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {

    }

    void AsyncListener::listenAsync(AcceptHandler asyncHandler)
    {
      acceptAsync(asyncHandler);

      mdebug_info("Listener started.\n");
      service_.run();
      mdebug_info("Listener finished.\n");
    }


    void AsyncListener::stop()
    {
      service_.stop();
    }

    void AsyncListener::acceptAsync(AcceptHandler asyncHandler)
    {
      auto connection = std::make_shared<AsyncConnection>(service_);

      auto proxyHandler = [this, connection, asyncHandler](const boost::system::error_code &err) {
        this->acceptAsync(asyncHandler);

        asyncHandler(connection, err);
      };

      acceptor_.async_accept(connection->getSocket(), proxyHandler);
    }
  }

}
