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

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace runnerd {

  namespace network {

    class AsyncConnection : public std::enable_shared_from_this<AsyncConnection>, boost::noncopyable {

        typedef AsyncConnection self_type;

      public:
        typedef std::shared_ptr<AsyncConnection> Ptr;

      public:
        AsyncConnection(boost::asio::io_service& service);

        boost::asio::ip::tcp::socket& getSocket();

        bool started() const;

        void onRead(const boost::system::error_code &err, size_t bytes);
        void doRead();

        void onWrite(const boost::system::error_code &err, size_t bytes);
        void doWrite(const std::string &msg);

        size_t readComplete(char *buf, const boost::system::error_code &err, size_t bytes);

      private:
        boost::asio::ip::tcp::socket socket_;

        char readBuffer_[UINT16_MAX];
        char writeBuffer_[UINT16_MAX];
        bool started_;

    };

  }

}




#endif //RUNNERD_ASYNCCONNECTION_HPP
