/*
 * Listener.cpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <memory>

#include <network/AsyncListener.hpp>

namespace runnerd {

  namespace network {


    AsyncListener::AsyncListener(int port):
            acceptor_(service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {

    }

    void AsyncListener::listenAsync()
    {
      acceptAsync();

      service_.run();
    }


    void AsyncListener::stop()
    {
      service_.stop();
    }

    void AsyncListener::handleAccept(AsyncConnection::Ptr connection, const boost::system::error_code& err)
    {

    }

    void AsyncListener::acceptAsync()
    {
      auto connection = std::make_shared<AsyncConnection>(service_);

      acceptor_.async_accept(connection->getSocket(), [this, connection](const boost::system::error_code &err)
      {
        this->handleAccept(connection, err);
      });
    }
  }

}
