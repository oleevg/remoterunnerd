/*
 * AsyncConnection.hpp
 *
 *  Created on: 10/3/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCCONNECTION_HPP
#define RUNNERD_ASYNCCONNECTION_HPP

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
     * @brief Asynchronous connection implementation using Boost.Asio IP TCP sockets.
     */
    class AsyncConnection : public IAsyncConnection, public std::enable_shared_from_this<AsyncConnection>, boost::noncopyable {

        typedef AsyncConnection self_type;

      public:
        typedef std::shared_ptr<AsyncConnection> Ptr;

      public:
        AsyncConnection(boost::asio::io_service& service);

        boost::asio::ip::tcp::socket& getSocket();

        bool isStarted() const;

        void close() override ;

        void readAsync(IOBuffer& buffer, ReadCompleteHandler readCompleteHandler, IOHandler readHandler) override ;
        void writeAsync(const IOBuffer& buffer, IOHandler handler) override ;

      private:
        boost::asio::ip::tcp::socket socket_;

        bool started_;

    };

  }

}




#endif //RUNNERD_ASYNCCONNECTION_HPP
