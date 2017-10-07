/*
 * CommandStore.hpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef C_COMMANDSTORE_HPP
#define C_COMMANDSTORE_HPP

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_set>

namespace runnerd {

  namespace common {

    class CommandStore {

      public:
        typedef std::shared_ptr<CommandStore> Ptr;
        typedef std::vector<std::string> CommandCollection;

      public:
        CommandStore(size_t sizeHint);

        bool registerCommand(const std::string& command);

        bool isRegistered(const std::string& command) const;

        CommandCollection getAllCommands() const;

        void setAllCommands(const CommandCollection& commands);

      private:
        std::unordered_set<std::string> commands_;
        mutable std::mutex mtx_;
    };

  }

}


#endif //C_COMMANDSTORE_HPP
