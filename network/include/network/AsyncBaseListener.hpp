/*
 * AsyncBaseListener.hpp
 *
 *  Created on: 10/8/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCBASELISTENER_HPP
#define RUNNERD_ASYNCBASELISTENER_HPP

#include <thread>
#include <list>

#include <boost/asio.hpp>

#include <core/ulog.h>

#include "IAsyncListener.hpp"

namespace runnerd {

  namespace network {

    /**
     * @brief Base asynchronous listener class.
     * @detailed Provides base asynchronous listener functionality using Boost.Asio.
     */
    template <class Acceptor>
    class AsyncBaseListener : public IAsyncListener {

      public:
        template <typename... Args>
        AsyncBaseListener(size_t threadPoolSize = 5, Args &&... args):
                threadPoolSize_(threadPoolSize), acceptor_(getIoService(), std::forward<Args>(args)...)
        { }

        virtual ~AsyncBaseListener() = default;

        virtual void wait() override
        {
          for (auto& serviceThread : threadPool_)
          {
            serviceThread.join();
          }

          mdebug_info("Listener finished.\n");
        }

        virtual void stop() override
        {
          service_.stop();
        }

        virtual void listenAsync(AcceptHandler asyncHandler) override
        {
          acceptAsync(asyncHandler);

          mdebug_info("Listener started.\n");
          for(size_t i = 0; i<threadPoolSize_; ++i)
          {
            threadPool_.emplace_back([this]() {
              getIoService().run();
            });
          }
        }

      protected:
        void acceptAsync(AcceptHandler asyncHandler)
        {
          auto connection = std::make_shared<AsyncLocalConnection>(getIoService());

          auto proxyHandler = [this, connection, asyncHandler](const boost::system::error_code &err) {
            this->acceptAsync(asyncHandler);

            asyncHandler(connection, err);
          };

          acceptor_.async_accept(connection->getSocket(), proxyHandler);
        }

        boost::asio::io_service& getIoService()
        {
          return service_;
        }

      private:
        Acceptor acceptor_;
        boost::asio::io_service service_;

        size_t threadPoolSize_;
        std::list<std::thread> threadPool_;
    };
  }

}



#endif //RUNNERD_ASYNCBASELISTENER_HPP
