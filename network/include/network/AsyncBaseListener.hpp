/*
 * AsyncBaseListener.hpp
 *
 *  Created on: 10/8/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCBASELISTENER_HPP
#define RUNNERD_ASYNCBASELISTENER_HPP

#include <boost/asio.hpp>

#include "IAsyncListener.hpp"

namespace runnerd {

  namespace network {

    class AsyncBaseListener : public IAsyncListener {
      public:
        virtual ~AsyncBaseListener() = default;

        virtual void stop() override;

        virtual void listenAsync(AcceptHandler asyncHandler) override;

      protected:
        boost::asio::io_service& getIoService();

      private:
        boost::asio::io_service service_;
    };
  }

}



#endif //RUNNERD_ASYNCBASELISTENER_HPP
