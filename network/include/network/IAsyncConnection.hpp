/*
 * IAsyncConnection.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace runnerd {

  namespace network {

    using Byte = uint8_t;
    using IOBuffer = std::vector<Byte>;

    using IOHandler = std::function<void(const boost::system::error_code&, size_t)>;
    using ReadCompleteHandler = std::function<size_t(const boost::system::error_code&, size_t)>;

    /**
     * @brief Asynchronous connection base interface.
     */
    class IAsyncConnection {
    public:
      using Ptr = std::shared_ptr<IAsyncConnection>;

    public:
      virtual ~IAsyncConnection() = default;

      /**
       * @brief Closes client's connection.
       */
      virtual void close() = 0;

      /**
       * @brief Asynchronous reading from client's socket.
       * @param buffer Buffer to write received data to.
       * @param readCompleteHandler Read complete functor.
       * @param readHandler Read data functor.
       */
      virtual void readAsync(IOBuffer& buffer, ReadCompleteHandler readCompleteHandler, IOHandler readHandler) = 0;

      /**
       * @brief Asynchronous writing to the client's socket.
       * @param buffer Buffer to read the data for sending from.
       * @param handler Write data functor.
       */
      virtual void writeAsync(const IOBuffer& buffer, IOHandler handler) = 0;
    };

  } // namespace network

} // namespace runnerd

