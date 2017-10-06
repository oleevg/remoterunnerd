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
        typedef std::shared_ptr<std::atomic<bool>> SignalHandlerFlag;
        typedef std::map<int, SignalHandlerFlag> SignalHandlerFlags;

      public:
        void daemonize();
        void setSignalHandler();

        bool unregisterSignalHandlerFlag(int signalNumber);
        void registerSignalHandlerFlag(int signalNumber, SignalHandlerFlag flag);

      private:
        SignalHandlerFlags signalHandlerFlags_;

    };

  }

}


#endif //RUNNERD_UNIXSERVICE_HPP
