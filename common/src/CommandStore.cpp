/*
 * CommandStore.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>

#include <common/CommandStore.hpp>

namespace runnerd {

  namespace common {

    CommandStore::CommandStore(size_t sizeHint)
    {
      commands_.reserve(sizeHint);
    }

    bool CommandStore::registerCommand(const std::string& command)
    {
      std::lock_guard<std::mutex> lock(mtx_);

      auto result = commands_.insert(command);

      mdebug_info("Registered command: %s", command.c_str());

      return result.second;
    }

    bool CommandStore::isRegistered(const std::string& command) const
    {
      std::lock_guard<std::mutex> lock(mtx_);

      return (commands_.count(command));
    }

    CommandStore::CommandCollection CommandStore::getAllCommands() const
    {
      std::lock_guard<std::mutex> lock(mtx_);

      CommandCollection output;
      output.reserve(commands_.size());

      for (const auto& command : commands_)
      {
        output.push_back(command);
      }

      return output;
    }

    void CommandStore::setAllCommands(const CommandStore::CommandCollection& commands)
    {
      std::lock_guard<std::mutex> lock(mtx_);

      commands_.clear();

      for (const auto& command : commands)
      {
        commands_.insert(command);
      }
    }

  }

}
