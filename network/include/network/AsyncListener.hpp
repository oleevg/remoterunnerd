/*
 * Listener.hpp
 *
 *  Created on: 10/2/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_LISTENER_HPP
#define RUNNERD_LISTENER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <network/AsyncConnection.hpp>

namespace runnerd {

  namespace network {

    class AsyncListener {

      public:
        AsyncListener(int port);

        void handleAccept(AsyncConnection::Ptr connection, const boost::system::error_code &err);

        void listenAsync();

        void stop();

      private:
        void acceptAsync();

      private:
        boost::asio::io_service service_;
        boost::asio::ip::tcp::acceptor acceptor_;

    };

  }

}


#endif //RUNNERD_LISTENER_HPP
