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

    /**
     * @brief Class responsible for keeping registered commands.
     */
    class CommandStore {

      public:
        typedef std::shared_ptr<CommandStore> Ptr;
        typedef std::vector<std::string> CommandCollection;

      public:
        /**
         * @brief ctor.
         * @param sizeHint Expected maximum number of stored items.
         */
        CommandStore(size_t sizeHint);

        bool registerCommand(const std::string& command);

        /**
         *
         * @param command Command name.
         * @return Whether command registered or not.
         */
        bool isRegistered(const std::string& command) const;

        /**
         * @brief Provides a thread-safe read access to all registered commands.
         * @return Custom collection of registered commands.
         */
        CommandCollection getAllCommands() const;

        /**
         * @brief Provides a thread-safe way to update all registered commands;
         * @param commands Custom collection of commands to register.
         */
        void setAllCommands(const CommandCollection& commands);

      private:
        std::unordered_set<std::string> commands_;
        mutable std::mutex mtx_;
    };

  }

}


#endif //C_COMMANDSTORE_HPP
