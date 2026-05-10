/*
 * BaseException.hpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#pragma once

#include <string>

namespace runnerd {

  namespace core {

    /**
     * @brief Applications's base exception class.
     */
    class BaseException : public std::exception {

    public:
      BaseException(const std::string& errorMessage);

      const char* what() const noexcept override;

    private:
      std::string errorMessage_;
    };

  } // namespace core

} // namespace runnerd

