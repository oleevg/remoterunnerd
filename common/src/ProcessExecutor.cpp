/*
 * ProcessExecutor.cpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <chrono>
#include <iostream>
#include <functional>
#include <thread>
#include <memory>

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <boost/process/async_pipe.hpp>
#include <boost/asio/io_service.hpp>

#include <core/ulog.h>
#include <core/ThreadHelper.hpp>
#include <core/BaseException.hpp>

#include <common/ProcessExecutor.hpp>
#include <boost/asio.hpp>

namespace runnerd {

  namespace common {

    typedef std::vector<char> BytesArray;

    class ProcessExecutorImpl
    {
      public:
        ProcessExecutorImpl();

        ~ProcessExecutorImpl();

        void run();

        void wait();

        std::string handleOutputStreamUsage() const;

        std::thread ioThread;
        boost::asio::io_service ioService;
        boost::process::async_pipe outPipe;
        boost::process::async_pipe errPipe;

        BytesArray outIoBuffer;
        BytesArray errIoBuffer;

        BytesArray outputData;
        BytesArray errorData;
    };

    ProcessExecutorImpl::ProcessExecutorImpl():
    outPipe(ioService), errPipe(ioService), outIoBuffer(1024), errIoBuffer(1024)
    {}

    void ProcessExecutorImpl::run()
    {
      ioThread = std::thread([this]() {
        ioService.run();
      });
    }

    ProcessExecutorImpl::~ProcessExecutorImpl()
    {
      wait();
    }

    void ProcessExecutorImpl::wait()
    {
      if(ioThread.joinable())
      {
        ioThread.join();
      }
    }

    std::string ProcessExecutorImpl::handleOutputStreamUsage() const
    {
      if(!outputData.empty())
      {
        return std::string(outputData.begin(), outputData.end());
      }

      return std::string(errorData.begin(), errorData.end());
    }

    // -----------------------------------------------------------------------

    const char* ProcessExecutor::Messages::Canceled = "Execution canceled due to the timeout expiration.";

    ProcessExecutor::ProcessExecutor()
    {
      impl = std::make_unique<ProcessExecutorImpl>();
    }

    ProcessExecutor::~ProcessExecutor() = default;

    std::string
    ProcessExecutor::executeProcess(const std::string& execName, const ProcessExecutor::Arguments& arguments,
                                    const std::chrono::milliseconds& timeout)
    {
      mdebug_info("Going to execute '%s'. ThreadId=0x%x", execName.c_str(), core::ThreadHelper::threadIdToInt());

      mdebug_info("Arguments:");
      for (const auto& arg : arguments)
      {
        mdebug_info("'%s' ", arg.c_str());
      }

      std::string execCommand = execName;
      for (const auto& arg : arguments)
      {
        execCommand.append(" ");
        execCommand.append(arg);
      }

      std::function<void(const boost::system::error_code & errorCode, std::size_t size, BytesArray& in, BytesArray& out, boost::process::async_pipe& pipe)> onDataReady;
      onDataReady = [&](const boost::system::error_code & errorCode, size_t size, BytesArray& in, BytesArray& out, boost::process::async_pipe& pipe)
      {
        out.reserve(out.size() + size);
        out.insert(out.end(), in.begin(), in.begin() + size);
        if (!errorCode)
        {
          boost::asio::async_read(pipe, boost::asio::buffer(in), [&](const boost::system::error_code & ec, size_t n)
          {
            onDataReady(ec, n, in, out, pipe);
          });
        }
      };

      std::function<void(const boost::system::error_code & errorCode, std::size_t size)> onOutputReady = [this, onDataReady](const boost::system::error_code & errorCode, size_t size)
      {
        mdebug_info("onOutputReady output received: size = %d, ec = %d", size, errorCode);
        onDataReady(errorCode, size, impl->outIoBuffer, impl->outputData, impl->outPipe);
      };

      std::function<void(const boost::system::error_code & errorCode, std::size_t size)> onErrorReady = [this, onDataReady](const boost::system::error_code & errorCode, size_t size)
      {
        mdebug_info("onErrorReady output received: size = %d, ec = %d", size, errorCode);
        onDataReady(errorCode, size, impl->errIoBuffer, impl->errorData, impl->errPipe);
      };

      try
      {
        boost::process::child childProcess(execCommand, boost::process::std_out > impl->outPipe,
                                           boost::process::std_err > impl->errPipe);

        boost::asio::async_read(impl->outPipe, boost::asio::buffer(impl->outIoBuffer), onOutputReady);
        boost::asio::async_read(impl->errPipe, boost::asio::buffer(impl->errIoBuffer), onErrorReady);

        impl->run();

        std::error_code errorCode;
        if (!childProcess.wait_for(timeout, errorCode))
        {
          mdebug_warn("Process '%s' execution timeout expired. Going to terminate the child process.", execName.c_str());
          childProcess.terminate();
          return Messages::Canceled;
        }

        int exitCode = childProcess.exit_code();

        impl->wait();

        if(exitCode)
        {
          mdebug_warn("Command '%s' execution failed. Return code: %d.", execName.c_str(), exitCode);
          return std::string(impl->errorData.begin(), impl->errorData.end());
        }

        return impl->handleOutputStreamUsage();
      }
      catch(const boost::process::process_error& exc)
      {
        throw core::BaseException(exc.what());
      }
    }
  }

}
