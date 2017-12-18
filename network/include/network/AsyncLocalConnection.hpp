/*
 * AsyncLocalConnection.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCLOCALCONNECTION_HPP
#define RUNNERD_ASYNCLOCALCONNECTION_HPP

#include "AsyncBaseConnection.hpp"

namespace runnerd {

  namespace network {

    /**
     * @brief Asynchronous connection implementation using Boost.Asio local sockets.
     */
      using AsyncLocalConnection = AsyncBaseConnection<boost::asio::local::stream_protocol::socket>;

  }

}




#endif //RUNNERD_ASYNCLOCALCONNECTION_HPP
