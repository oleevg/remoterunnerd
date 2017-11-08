/*
 * AsyncConnection.cpp
 *
 *  Created on: 10/3/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>

#include <network/AsyncConnection.hpp>


namespace runnerd {

  namespace network {

    AsyncConnection::AsyncConnection(boost::asio::io_service& service) :
            socket_(service), started_(true)
    {

    }

    void AsyncConnection::readAsync(IOBuffer& buffer, ReadCompleteHandler readCompleteHandler, IOHandler readHandler)
    {
      if (!isStarted()) return;

      boost::asio::async_read(getSocket(), boost::asio::buffer(buffer), readCompleteHandler, readHandler);
    }

    void AsyncConnection::writeAsync(const IOBuffer& buffer, IOHandler handler)
    {
      if (!isStarted()) return;

      getSocket().async_write_some(boost::asio::buffer(buffer), handler);
    }

    boost::asio::ip::tcp::socket& AsyncConnection::getSocket()
    {
      return socket_;
    }

    bool AsyncConnection::isStarted() const
    {
      return started_;
    }

    void AsyncConnection::close()
    {
      started_ = false;
      socket_.close();
    }
  }

}
