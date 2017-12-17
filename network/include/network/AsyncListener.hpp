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
    class AsyncListener : public AsyncBaseListener {

      public:
        typedef std::shared_ptr<AsyncListener> Ptr;

      public:
        explicit AsyncListener(int port, size_t threadPoolSize);

      private:
        void acceptAsync(AcceptHandler asyncHandler) override ;

      private:
        boost::asio::ip::tcp::acceptor acceptor_;

    };

  }

}


#endif //RUNNERD_LISTENER_HPP
