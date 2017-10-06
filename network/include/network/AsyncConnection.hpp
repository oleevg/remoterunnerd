/*
 * AsyncConnection.hpp
 *
 *  Created on: 10/3/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCCONNECTION_HPP
#define RUNNERD_ASYNCCONNECTION_HPP

#include <cstdint>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace runnerd {

  namespace network {

    class AsyncConnection : public std::enable_shared_from_this<AsyncConnection>, boost::noncopyable {

        typedef AsyncConnection self_type;

      public:
        typedef std::shared_ptr<AsyncConnection> Ptr;
        typedef std::function<void(const boost::system::error_code&, size_t)> IOHandler;
        typedef std::function<size_t(const boost::system::error_code&, size_t)> ReadCompleteHandler;

      public:
        AsyncConnection(boost::asio::io_service& service);

        boost::asio::ip::tcp::socket& getSocket();

        bool isStarted() const;

        void readAsync(char* buffer, size_t size, ReadCompleteHandler readCompleteHandler, IOHandler readHandler);
        void writeAsync(const std::string& msg, IOHandler handler);

      private:
        boost::asio::ip::tcp::socket socket_;

        bool started_;

    };

  }

}




#endif //RUNNERD_ASYNCCONNECTION_HPP
