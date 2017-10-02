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
      auto result = commands_.insert(command);

      mdebug_info("Registered command: %s", command.c_str());

      return result.second;
    }

    bool CommandStore::isRegistered(const std::string& command) const
    {
      return (commands_.count(command));
    }

  }

}
