/*
 * CommandStore_test.cpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#define BOOST_TEST_MODULE "CommandStore test module"

#include <boost/test/unit_test.hpp>

#include <common/CommandStore.hpp>

BOOST_AUTO_TEST_SUITE (CommandStore)
  
  BOOST_AUTO_TEST_CASE(setAllCommands_isRegistered_true)
  {
    runnerd::common::CommandStore commandStore(10);

    runnerd::common::CommandStore::CommandCollection commands = {"date", "pwd"};
    commandStore.setAllCommands(commands);

    for (const auto& command : commands)
    {
      BOOST_REQUIRE(commandStore.isRegistered(command));
    }
  }

  BOOST_AUTO_TEST_CASE(updateCommands_isRegistered_false)
  {
    runnerd::common::CommandStore commandStore(10);

    runnerd::common::CommandStore::CommandCollection commands = {"date", "pwd"};
    commandStore.setAllCommands(commands);

    commandStore.setAllCommands({"ls"});

    for (const auto& command : commands)
    {
      BOOST_REQUIRE(!commandStore.isRegistered(command));
    }
  }

BOOST_AUTO_TEST_SUITE_END()