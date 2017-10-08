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

    void AsyncLocalConnection::readAsync(char* buffer, size_t size, ReadCompleteHandler readCompleteHandler,
                                    IOHandler readHandler)
    {
      boost::asio::async_read(getSocket(), boost::asio::buffer(buffer, size), readCompleteHandler, readHandler);
    }

    void AsyncLocalConnection::writeAsync(const std::string& msg, IOHandler handler)
    {
      if (!isStarted()) return;

      mdebug_info("Going to write %s.", msg.c_str());

      getSocket().async_write_some(boost::asio::buffer(msg.c_str(), msg.length()), handler);
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
      socket_.close();
    }

  }


}