/*
 * CommandStore.hpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#ifndef C_COMMANDSTORE_HPP
#define C_COMMANDSTORE_HPP

#include <string>
#include <memory>
#include <unordered_set>

namespace runnerd {

  namespace common {

    class CommandStore {

      public:
        typedef std::shared_ptr<CommandStore> Ptr;

      public:
        CommandStore(size_t sizeHint);

        bool registerCommand(const std::string& command);

        bool isRegistered(const std::string& command) const;

      private:
        std::unordered_set<std::string> commands_;
    };

  }

}


#endif //C_COMMANDSTORE_HPP
