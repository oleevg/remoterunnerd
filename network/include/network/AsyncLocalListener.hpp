/*
 * AsyncLocalListener.hpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_ASYNCLOCALLISTENER_HPP
#define RUNNERD_ASYNCLOCALLISTENER_HPP

#include <boost/system/error_code.hpp>

#include "AsyncBaseListener.hpp"

namespace runnerd {

  namespace network {

    class AsyncLocalListener : public AsyncBaseListener  {
      public:
        typedef std::shared_ptr<AsyncLocalListener> Ptr;

      public:
        AsyncLocalListener(const std::string& socketPath);

        void acceptAsync(AcceptHandler asyncHandler) override;

      private:
        boost::asio::local::stream_protocol::acceptor acceptor_;
    };

  }

}


#endif //RUNNERD_ASYNCLOCALLISTENER_HPP
