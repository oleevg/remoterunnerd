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
#include <mutex>
#include <condition_variable>

#include <boost/serialization/singleton.hpp>

namespace runnerd {

  namespace common {

    /**
     * @brief Unix service class.
     * @detailed Provides running processes as a daemon. Provides API to register signals to be handled.
     */
    class UnixService : public boost::serialization::singleton<UnixService> {

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
        /**
         * @brief Deamonizes the service.
         */
        void daemonize();

        /**
         * @brief Sets signal handler function.
         * @detailed Must be called after all required signals have been registered.
         */
        void setSignalHandler();

        /**
         * @brief Blocks the calling thread and waits for a signal.
         */
        void waitForSignalsSync();

        /**
         * @brief Unregisteres previously registered signal.
         * @param signalNumber Signal number to unregister.
         * @return Whether the specified signal was successfully unregistered.
         */
        bool unregisterSignalHandlerFlag(int signalNumber);

        /**
         * @brief Registeres the specified signal.
         * @param signalNumber Signal number to register.
         * @param flag Atomic flag to keep the signal status.
         */
        void registerSignalHandlerFlag(int signalNumber, SignalHandlerFlag::Ptr flag);

      private:
        static void signalHandler(int signalNumber);

      private:
        static SignalHandlerFlags& getSignalHandlerFlags();

        static std::mutex mtx;
        static std::condition_variable conditionVariable;
    };

  }

}


#endif //RUNNERD_UNIXSERVICE_HPP
