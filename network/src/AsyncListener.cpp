/*
 * AsyncListener.cpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <memory>
#include <thread>

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>

#include <network/AsyncListener.hpp>
#include <network/AsyncConnection.hpp>


namespace runnerd {

  namespace network {

    AsyncListener::AsyncListener(int port, size_t threadPoolSize) :
            AsyncBaseListener(threadPoolSize), acceptor_(getIoService(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {

    }

    void AsyncListener::acceptAsync(AcceptHandler asyncHandler)
    {
      auto connection = std::make_shared<AsyncConnection>(getIoService());

      auto proxyHandler = [this, connection, asyncHandler](const boost::system::error_code &err) {
        this->acceptAsync(asyncHandler);

        asyncHandler(connection, err);
      };

      acceptor_.async_accept(connection->getSocket(), proxyHandler);
    }
  }

}
