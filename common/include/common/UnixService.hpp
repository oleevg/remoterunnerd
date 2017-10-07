/*
 * UnixService.hpp
 *
 *  Created on: 10/5/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef RUNNERD_UNIXSERVICE_HPP
#define RUNNERD_UNIXSERVICE_HPP

#include <atomic>
#include <memory>
#include <map>

namespace runnerd {

  namespace common {

    class UnixService {

      public:
        struct SignalHandlerFlag {
          typedef std::shared_ptr<SignalHandlerFlag> Ptr;

          SignalHandlerFlag(bool value) : flag (value)
          {

          }

          std::atomic<bool> flag;
        };

        typedef std::map<int, SignalHandlerFlag::Ptr> SignalHandlerFlags;

      public:
        void daemonize();
        void setSignalHandler();

        bool unregisterSignalHandlerFlag(int signalNumber);
        void registerSignalHandlerFlag(int signalNumber, SignalHandlerFlag::Ptr flag);

      private:
        static void signalHandler(int signalNumber);

      private:
        static SignalHandlerFlags& getSignalHandlerFlags();

    };

  }

}


#endif //RUNNERD_UNIXSERVICE_HPP
