/*
 * ProcessExecutor_test.cpp
 *
 *  Created on: 11/16/19
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#define BOOST_TEST_MODULE "ProcessExecutor test module"

#include <string>
#include <vector>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include <common/ProcessExecutor.hpp>
#include <core/BaseException.hpp>

BOOST_AUTO_TEST_SUITE(ProcessExecutor)

  BOOST_AUTO_TEST_CASE(Most_throw_when_app_is_not_in_path)
  {
    runnerd::common::ProcessExecutor processExecutor;

    #ifdef _WIN32
    std::string processName = "ProcessExecutor_test.exe";
    #else
    std::string processName = "ProcessExecutor_test";
    #endif

    BOOST_REQUIRE_THROW(processExecutor.executeProcess(processName, {}, std::chrono::seconds(1)), runnerd::core::BaseException);
  }

BOOST_AUTO_TEST_CASE(Should_read_process_output)
  {
    runnerd::common::ProcessExecutor processExecutor;

    std::string processName = "python";
    std::vector<std::string> arguments{"--version"};

    auto result = processExecutor.executeProcess(processName, arguments, std::chrono::seconds(3));

    std::cout << "Process output:" << result << std::endl;

    auto flag = result.find("Python");
    BOOST_REQUIRE(flag != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(Should_handle_execution_timeout)
  {
    runnerd::common::ProcessExecutor processExecutor;

    std::string processName = "python";
    std::vector<std::string> arguments;

    auto result = processExecutor.executeProcess(processName, arguments, std::chrono::seconds(3));

    std::cout << "Process output:" << result << std::endl;

    BOOST_REQUIRE_EQUAL(result, runnerd::common::ProcessExecutor::Messages::Canceled);
  }

BOOST_AUTO_TEST_SUITE_END()

