/*
 * CommandStore_test.cpp
 *
 *  Created on: 10/7/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <boost/test/unit_test.hpp>

#include <common/CommandStore.hpp>

BOOST_AUTO_TEST_SUITE(CommandStore)

BOOST_AUTO_TEST_CASE(Should_keep_registered_command)
{
  runnerd::common::CommandStore commandStore(10);

  runnerd::common::CommandStore::CommandCollection commands = {"date", "pwd"};
  commandStore.setAllCommands(commands);

  for (const auto& command : commands)
  {
    BOOST_REQUIRE(commandStore.isRegistered(command));
  }
}

BOOST_AUTO_TEST_CASE(Should_update_commands)
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

BOOST_AUTO_TEST_CASE(Should_register_command_by_one)
{
  runnerd::common::CommandStore commandStore(10);

  runnerd::common::CommandStore::CommandCollection commands = {"date", "pwd"};
  commandStore.setAllCommands(commands);

  commandStore.registerCommand("ls");

  BOOST_REQUIRE(commandStore.getAllCommands().size() == (commands.size() + 1));
  BOOST_REQUIRE(commandStore.isRegistered("ls"));
}

BOOST_AUTO_TEST_SUITE_END()