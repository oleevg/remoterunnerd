/*
 * Listener.hpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_LISTENER_HPP
#define RUNNERD_LISTENER_HPP

#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <network/AsyncConnection.hpp>

namespace runnerd {

  namespace network {

    class AsyncListener {

      public:
        typedef std::shared_ptr<AsyncListener> Ptr;
        typedef std::function<void(const AsyncConnection::Ptr&, const boost::system::error_code&)> AcceptHandler;

      public:
        AsyncListener(int port);

        void listenAsync(AcceptHandler asyncHandler);

        void stop();

        void acceptAsync(AcceptHandler asyncHandler);

      private:
        boost::asio::io_service service_;
        boost::asio::ip::tcp::acceptor acceptor_;

    };

  }

}


#endif //RUNNERD_LISTENER_HPP
