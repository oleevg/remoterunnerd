/*
 * IAsyncConnection.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_IASYNCCONNECTION_HPP
#define RUNNERD_IASYNCCONNECTION_HPP

#include <string>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace runnerd {

  namespace network {

    typedef std::function<void(const boost::system::error_code&, size_t)> IOHandler;
    typedef std::function<size_t(const boost::system::error_code&, size_t)> ReadCompleteHandler;

    class IAsyncConnection {
      public:
        typedef std::shared_ptr<IAsyncConnection> Ptr;

      public:

        virtual ~IAsyncConnection() = default;

        virtual void close() = 0;

        virtual void readAsync(char* buffer, size_t size, ReadCompleteHandler readCompleteHandler, IOHandler readHandler) = 0;
        virtual void writeAsync(const std::string& msg, IOHandler handler) = 0;

    };

  }

}

#endif //RUNNERD_IASYNCCONNECTION_HPP
