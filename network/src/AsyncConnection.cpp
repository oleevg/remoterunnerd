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

    void AsyncConnection::readAsync(char* buffer, size_t size, ReadCompleteHandler readCompleteHandler,
                                    IOHandler readHandler)
    {
      if (!isStarted()) return;

      boost::asio::async_read(getSocket(), boost::asio::buffer(buffer, size), readCompleteHandler, readHandler);
    }

    void AsyncConnection::writeAsync(const std::string& msg, IOHandler handler)
    {
      if (!isStarted()) return;

      getSocket().async_write_some(boost::asio::buffer(msg.c_str(), msg.length()), handler);
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
