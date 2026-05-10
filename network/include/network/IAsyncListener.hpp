/*
 * IAsyncListener.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <memory>
#include <functional>

#include <boost/system/error_code.hpp>

#include "IAsyncConnection.hpp"

namespace runnerd {

  namespace network {

    using AcceptHandler = std::function<void(const IAsyncConnection::Ptr&, const boost::system::error_code&)>;

    /**
     * @brief Asynchronous listener base interface.
     */
    class IAsyncListener {
    public:
      using Ptr = std::shared_ptr<IAsyncListener>;

    public:
      virtual ~IAsyncListener() = default;

      /**
       * @brief Starts to listen on the socket.
       * @param asyncHandler Callable object to process client's connections.
       */
      virtual void listenAsync(AcceptHandler asyncHandler) = 0;

      /**
       * @brief Waits for the server to stop listening.
       */
      virtual void wait() = 0;

      /**
       * @brief Stops listening on the used socket.
       */
      virtual void stop() = 0;

    protected:
      /**
       * @brief Starts waiting for client's connections.
       * @param asyncHandler Callable object to process client's connections
       */
      virtual void acceptAsync(AcceptHandler asyncHandler) = 0;
    };

  } // namespace network

} // namespace runnerd

