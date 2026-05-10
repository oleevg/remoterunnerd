/*
 * AsyncListener.hpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <boost/asio.hpp>

#include <core/ulog.h>

#include "AsyncBaseListener.hpp"

namespace runnerd {

  namespace network {

    /**
     * @brief Asynchronous listener implementation using Boost.Asio IP TCP sockets.
     */
    class AsyncListener : public AsyncBaseListener<boost::asio::ip::tcp::acceptor, boost::asio::ip::tcp::socket> {
    public:
      AsyncListener(size_t threadPoolSize, int port)
          : AsyncBaseListener(threadPoolSize, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
      {
        mdebug_info("Running with the port '%d'.", port);
      }
    };

  } // namespace network

} // namespace runnerd

