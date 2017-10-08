/*
 * AsyncBaseListener.cpp
 *
 *  Created on: 10/8/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>

#include <network/AsyncBaseListener.hpp>

namespace runnerd {

  namespace network {

    void AsyncBaseListener::stop()
    {
      service_.stop();
    }

    void AsyncBaseListener::listenAsync(AcceptHandler asyncHandler)
    {
      acceptAsync(asyncHandler);

      mdebug_info("Listener started.\n");
      getIoService().run();
      mdebug_info("Listener finished.\n");
    }

    boost::asio::io_service& AsyncBaseListener::getIoService()
    {
      return service_;
    }

  }

}
