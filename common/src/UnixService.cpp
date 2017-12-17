/*
 * UnixService.cpp
 *
 *  Created on: 10/5/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include <boost/format.hpp>

#include <core/ulog.h>
#include <core/SystemException.hpp>

#include <common/UnixService.hpp>


namespace runnerd {

  namespace common {

    std::mutex UnixService::mtx;
    std::condition_variable UnixService::conditionVariable;

    void UnixService::daemonize()
    {
      FILE* fStream = nullptr;

      fprintf( stdout, "Daemonizing the process...\n");
      pid_t pid = fork();

      switch(pid)
      {
        case 0:
          /* Child process */
          /* On success: The child process becomes session leader */
          if (setsid() < 0)
          {
            throw core::SystemException((boost::format("Error in setsid() call: %s") % strerror(errno)).str(), errno);
          }

          /* Catch, ignore and handle signals */
          signal(SIGCHLD, SIG_IGN);
          signal(SIGHUP, SIG_IGN);

          /* Fork off for the second time*/
          pid = fork();

          /* An error occurred */
          if (pid < 0)
          {
            throw core::SystemException(
                    (boost::format("Error in fork() when spawning a grandchild process: %s") % strerror(errno)).str(), errno);
          }

          /* Success: Let the parent terminate */
          if (pid > 0)
          {
            exit(EXIT_SUCCESS);
          }

          /* Set new file permissions */
          umask(0);

          /* Change the working directory to the root directory */
          /* or another appropriated directory */
          chdir("/");

          fStream  = freopen("/dev/null", "r+", stdin);
          if (!fStream)
          {
            throw core::BaseException("Cannot attach stdin to /dev/null");
          }

          fStream = freopen("/dev/null", "r+", stdout);
          if (!fStream)
          {
            throw core::BaseException("Cannot attach stdout to /dev/null");
          }

          fStream = freopen("/dev/null", "r+", stderr);
          if (!fStream)
          {
            throw core::BaseException("Cannot attach stderr to /dev/null");
          }

          break;

        case -1:
          throw core::SystemException(
                  (boost::format("Error in fork() when spawning child process: %s") % strerror(errno)).str(), errno);

        default:
          /* The main process */
          exit(EXIT_SUCCESS);

      }
    }

    void UnixService::registerSignalHandlerFlag(int signalNumber, SignalHandlerFlag::Ptr flag)
    {
      if(getSignalHandlerFlags().count(signalNumber))
      {
        throw core::BaseException(
                "Flag for signal %d has already been registered. You have to unregister it at first to continue.");
      }

      getSignalHandlerFlags()[signalNumber] = flag;
    }

    bool UnixService::unregisterSignalHandlerFlag(int signalNumber)
    {
      return getSignalHandlerFlags().erase(signalNumber);
    }

    void UnixService::setSignalHandler()
    {
      struct sigaction sa;

      memset(&sa, 0, sizeof(sa));
      sa.sa_handler = &signalHandler;
      sa.sa_flags = SA_RESTART;

      for (const auto& sh : getSignalHandlerFlags() )
      {
        sigaction(sh.first, &sa, nullptr);
      }
    }

    void UnixService::signalHandler(int signalNumber)
    {
      for (auto item : getSignalHandlerFlags())
      {
        if(signalNumber == item.first)
        {
          auto& flag = item.second->flag;
          flag.store(true);

          std::unique_lock<std::mutex> lck(mtx);
          conditionVariable.notify_one();
        }
      }
    }

    UnixService::SignalHandlerFlags& UnixService::getSignalHandlerFlags()
    {
      static SignalHandlerFlags signalHandlerFlags;

      return signalHandlerFlags;
    }

    void UnixService::waitForSignalsSync()
    {
      std::unique_lock<std::mutex> lck(mtx);
      conditionVariable.wait(lck);
    }
  }

}
