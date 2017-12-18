/*
 * AsyncListener.hpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_LISTENER_HPP
#define RUNNERD_LISTENER_HPP

#include <memory>

#include <boost/asio.hpp>

#include "AsyncBaseListener.hpp"

namespace runnerd {

  namespace network {

    /**
     * @brief Asynchronous listener implementation using Boost.Asio IP TCP sockets.
     */

    template<class Acceptor>
    class AsyncListener : public AsyncBaseListener<Acceptor> {};

    template <>
    class AsyncListener<boost::asio::ip::tcp::acceptor> : public AsyncBaseListener<boost::asio::ip::tcp::acceptor> {

      public:
        typedef std::shared_ptr<AsyncListener> Ptr;

      public:
        AsyncListener(size_t threadPoolSize, int port):
                AsyncBaseListener(threadPoolSize, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
        { }
    };

  }

}


#endif //RUNNERD_LISTENER_HPP
