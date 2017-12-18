/*
 * AsyncBaseConnection.h
 *
 *  Created on: 12/18/2017
 *      Author: fedorov2
 */
#ifndef RUNNERD_ASYNCBASECONNECTION_H
#define RUNNERD_ASYNCBASECONNECTION_H

#include <boost/asio.hpp>

#include "IAsyncConnection.hpp"

namespace runnerd {

  namespace network {

    template <class Socket>
    class AsyncBaseConnection : public IAsyncConnection {
      public:
        explicit AsyncBaseConnection(boost::asio::io_service& service):
        socket_(service), started_(true)
        { }

        Socket& getSocket()
        {
          return socket_;
        }

        bool isStarted() const
        {
          return started_;
        }

        void close() override
        {
          started_ = false;
          socket_.close();
        }

        void readAsync(IOBuffer& buffer, ReadCompleteHandler readCompleteHandler, IOHandler readHandler) override
        {
          if (!isStarted()) return;

          boost::asio::async_read(getSocket(), boost::asio::buffer(buffer), readCompleteHandler, readHandler);
        }

        void writeAsync(const IOBuffer& buffer, IOHandler handler) override
        {
          if (!isStarted()) return;

          getSocket().async_write_some(boost::asio::buffer(buffer), handler);
        }

      private:
        Socket socket_;

        bool started_;

    };

  }

}

#endif //RUNNERD_ASYNCBASECONNECTION_H
