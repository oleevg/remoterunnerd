/*
 * IAsyncListener.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_IASYNCLISTENER_HPP
#define RUNNERD_IASYNCLISTENER_HPP

#include <memory>
#include <functional>

#include <boost/system/error_code.hpp>

#include "IAsyncConnection.hpp"

namespace runnerd {

  namespace network {

    typedef std::function<void(const IAsyncConnection::Ptr&, const boost::system::error_code&)> AcceptHandler;

    class IAsyncListener {
      public:
        typedef std::shared_ptr<IAsyncListener> Ptr;

      public:
        virtual ~IAsyncListener() = default;

        virtual void listenAsync(AcceptHandler asyncHandler) = 0;

        virtual void stop() = 0;

      protected:
        virtual void acceptAsync(AcceptHandler asyncHandler) = 0;
    };

  }

}

#endif //RUNNERD_IASYNCLISTENER_HPP
