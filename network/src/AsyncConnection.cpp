/*
 * AsyncConnection.cpp
 *
 *  Created on: 10/3/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <boost/bind.hpp>

#include <network/AsyncConnection.hpp>

#define MEM_FN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN2(x, y, z) boost::bind(&self_type::x, shared_from_this(),y,z)

namespace runnerd {

  namespace network {

    AsyncConnection::AsyncConnection(boost::asio::io_service& service):
    socket_(service), started_(false)
    {

    }

    void AsyncConnection::onRead(const boost::system::error_code &err, size_t bytes)
    {
      if (!err) {
        std::string msg(readBuffer_, bytes);
        doWrite(msg + "\n");
      }
    }

    void AsyncConnection::onWrite(const boost::system::error_code &err, size_t bytes)
    {
      doRead();
    }

    size_t AsyncConnection::readComplete(char *buf, const boost::system::error_code &err, size_t bytes)
    {
      if (err) return 0;
      bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
      // we read one-by-one until we get to enter, no buffering
      return found ? 0 : 1;
    }

    void AsyncConnection::doRead()
    {
      boost::asio::async_read(getSocket(), boost::asio::buffer(readBuffer_, sizeof(readBuffer_)), boost::bind(&AsyncConnection::readComplete, shared_from_this(), readBuffer_, _1, _2), MEM_FN2(onRead, _1, _2));
    }

    void AsyncConnection::doWrite(const std::string &msg)
    {
      if (!started()) return;
      std::copy(msg.begin(), msg.end(), writeBuffer_);
      getSocket().async_write_some(boost::asio::buffer(writeBuffer_, msg.size()), MEM_FN2(onWrite, _1, _2));
    }

    boost::asio::ip::tcp::socket& AsyncConnection::getSocket()
    {
      return socket_;
    }

    bool AsyncConnection::started() const
    {
      return started_;
    }
  }

}
