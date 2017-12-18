/*
 * AsyncLocalListener.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCLOCALLISTENER_HPP
#define RUNNERD_ASYNCLOCALLISTENER_HPP

#include <boost/system/error_code.hpp>

#include "AsyncBaseListener.hpp"

namespace runnerd {

  namespace network {

    /**
     * @brief Asynchronous listener implementation using Boost.Asio local sockets.
     */

    template<class Acceptor>
    class AsyncLocalListener : public AsyncBaseListener<Acceptor> {};

    template<>
    class AsyncLocalListener<boost::asio::local::stream_protocol::acceptor> : public AsyncBaseListener<boost::asio::local::stream_protocol::acceptor>  {
      public:
        typedef std::shared_ptr<AsyncBaseListener> Ptr;

      public:
        AsyncLocalListener(size_t threadPoolSize, const std::string& unixSocketPath):
                AsyncBaseListener(threadPoolSize, boost::asio::local::stream_protocol::endpoint(unixSocketPath)), unixSocketPath_(unixSocketPath)
        { }

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
