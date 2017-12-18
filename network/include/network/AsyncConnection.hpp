/*
 * AsyncConnection.hpp
 *
 *  Created on: 10/3/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCCONNECTION_HPP
#define RUNNERD_ASYNCCONNECTION_HPP

#include "AsyncBaseConnection.hpp"

namespace runnerd {

  namespace network {

    /**
    * @brief Asynchronous connection implementation using Boost.Asio IP TCP sockets.
    */
    using AsyncConnection = AsyncBaseConnection<boost::asio::ip::tcp::socket>;

  }

}




#endif //RUNNERD_ASYNCCONNECTION_HPP
