/*
 * AsyncLocalListener.cpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <cstdio>

#include <core/ulog.h>

#include <network/AsyncLocalListener.hpp>
#include <network/AsyncLocalConnection.hpp>

namespace runnerd {

  namespace network {

    AsyncLocalListener::AsyncLocalListener(const std::string& unixSocketPath):
            acceptor_(getIoService(), boost::asio::local::stream_protocol::endpoint(unixSocketPath))
    {

    }

    void AsyncLocalListener::acceptAsync(AcceptHandler asyncHandler)
    {
      auto connection = std::make_shared<AsyncLocalConnection>(getIoService());

      auto proxyHandler = [this, connection, asyncHandler](const boost::system::error_code &err) {
        this->acceptAsync(asyncHandler);

        asyncHandler(connection, err);
      };

      acceptor_.async_accept(connection->getSocket(), proxyHandler);
    }

    AsyncLocalListener::~AsyncLocalListener()
    {
      remove(unixSocketPath_.c_str());
    }

  }

}
