/*
 * ProcessExecutor.cpp
 *
 *  Created on: 10/6/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <chrono>

#include <boost/process.hpp>

#include <core/ulog.h>

#include <common/ProcessExecutor.hpp>

namespace runnerd {

namespace common {

	std::string ProcessExecutor::executeProcess(const std::string& execName, const ProcessExecutor::Arguments& arguments, int timeout)
	{
		// boost::process::context ctx; 
		// ctx.stdout_behavior = boost::process::capture_stream(); 
		// ctx.environment = boost::process::self::get_environment();
		
		mdebug_info("Going to execute '%s'", execName.c_str());

		for (auto c: execName)
		{
			mdebug_info("%x ", c);	
		}

		for (const auto& arg : arguments)
		{
			mdebug_info("'%s' ", arg.c_str());
		}

		boost::process::ipstream outputStream;
		//boost::process::child childProcess = boost::process::launch(execName, arguments, ctx); 
		boost::process::child childProcess(execName, arguments, boost::process::std_out > outputStream );

		std::error_code errorCode;
		if(!childProcess.wait_for(std::chrono::seconds(timeout), errorCode))
		{
			childProcess.terminate();
		}

		// if (childProcess.exited()) 
		// {
		// 	mdebug_info("Program returned exit code %d.", childProcess.exit_status()); 	
		// }
	 //    else if (childProcess.signaled()) 
	 //    { 
	 //        mdebug_info("Program received signal %d.", childProcess.term_signal()); 
	 //    } 
	 //    else if (childProcess.stopped()) 
	 //    {
	 //        mdebug_info("Program stopped by signal %d.", childProcess.stop_signal()); 
	 //    }
	 //    else 
	 //    {
	 //        mdebug_info("Unknown termination reason"); 
	 //    }

	    mdebug_info("Child process's output by line:");
    	std::string line; 
    	while (std::getline(outputStream, line)) 
        	mdebug_info(line.c_str());

    	return std::string();
	}

}

}
