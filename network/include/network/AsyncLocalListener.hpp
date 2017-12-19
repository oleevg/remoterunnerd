/*
 * AsyncLocalListener.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCLOCALLISTENER_HPP
#define RUNNERD_ASYNCLOCALLISTENER_HPP

#include <string>
#include <cstdio>

#include <core/ulog.h>

#include "AsyncBaseListener.hpp"

namespace runnerd {

  namespace network {

    /**
     * @brief Asynchronous listener implementation using Boost.Asio local sockets.
     */
    class AsyncLocalListener
            : public AsyncBaseListener<boost::asio::local::stream_protocol::acceptor, boost::asio::local::stream_protocol::socket> {
      public:
        AsyncLocalListener(size_t threadPoolSize, const std::string& unixSocketPath) :
                AsyncBaseListener(threadPoolSize, boost::asio::local::stream_protocol::endpoint(unixSocketPath)),
                unixSocketPath_(unixSocketPath)
        {
          mdebug_info("Running with the local socket '%s'.", unixSocketPath_.c_str());
        }

        ~AsyncLocalListener()
        {
          remove(unixSocketPath_.c_str());
        }

      private:
        std::string unixSocketPath_;
    };

  }

}


#endif //RUNNERD_ASYNCLOCALLISTENER_HPP
