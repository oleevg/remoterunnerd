/*
 * AsyncLocalConnection.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCLOCALCONNECTION_HPP
#define RUNNERD_ASYNCLOCALCONNECTION_HPP

#include <cstdint>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/system/error_code.hpp>

#include "IAsyncConnection.hpp"

namespace runnerd {

  namespace network {

    /**
     * @brief Asynchronous connection implementation using Boost.Asio local sockets.
     */
    class AsyncLocalConnection : public IAsyncConnection, public std::enable_shared_from_this<AsyncLocalConnection>, boost::noncopyable {
        typedef AsyncLocalConnection self_type;

      public:
        typedef std::shared_ptr<AsyncLocalConnection> Ptr;

      public:
        AsyncLocalConnection(boost::asio::io_service& service);

        boost::asio::local::stream_protocol::socket& getSocket();

        bool isStarted() const;

        void close();

        void readAsync(char* buffer, size_t size, ReadCompleteHandler readCompleteHandler, IOHandler readHandler);
        void writeAsync(const std::string& msg, IOHandler handler);

      private:
        boost::asio::local::stream_protocol::socket socket_;

        bool started_;

    };
  }

}




#endif //RUNNERD_ASYNCLOCALCONNECTION_HPP
