/*
 * AsyncLocalConnection.cpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>

#include <network/AsyncLocalConnection.hpp>

namespace runnerd {

  namespace network {

    AsyncLocalConnection::AsyncLocalConnection(boost::asio::io_service& service) :
            socket_(service), started_(true)
    {

    }

    void AsyncLocalConnection::readAsync(IOBuffer& buffer, ReadCompleteHandler readCompleteHandler, IOHandler readHandler)
    {
      if (!isStarted()) return;

      boost::asio::async_read(getSocket(), boost::asio::buffer(buffer), readCompleteHandler, readHandler);
    }

    void AsyncLocalConnection::writeAsync(const IOBuffer& buffer, IOHandler handler)
    {
      if (!isStarted()) return;

      getSocket().async_write_some(boost::asio::buffer(buffer), handler);
    }

    boost::asio::local::stream_protocol::socket& AsyncLocalConnection::getSocket()
    {
      return socket_;
    }

    bool AsyncLocalConnection::isStarted() const
    {
      return started_;
    }

    void AsyncLocalConnection::close()
    {
      started_  =false;
      socket_.close();
    }

  }


}