/*
 * TextConfigurationParser_test.cpp
 *
 *  Created on: 12/17/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#define BOOST_TEST_MODULE "TextConfigurationParser test module"

#include <boost/test/unit_test.hpp>
#include <fstream>
#include <random>
#include <unistd.h>

#include <common/TextConfigurationParser.hpp>
#include <core/BaseException.hpp>

namespace {

  std::string generateRandomString(size_t length)
  {
    const char symbols[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static std::mt19937 rg{std::random_device{}()};
    static std::uniform_int_distribution<size_t> pick(0, sizeof(symbols) - 2);

    std::string result;
    result.reserve(length);

    while(length-- > 0)
    {
      result += symbols[pick(rg)];
    }

    return result;
  }

}

struct TextConfigurationParserFixture {

  TextConfigurationParserFixture():
          isOwner_(false)
  {
    while (true)
    {
      fileName_ = generateRandomString(17);
      if(access(fileName_.c_str(), F_OK))
      {
        break;
      }
    }
  }

  ~TextConfigurationParserFixture()
  {
    if(isOwner_ && !fileName_.empty())
    {
      remove(fileName_.c_str());
    }
  }

  bool createFile()
  {
    std::ofstream outfile(fileName_);
    isOwner_ = outfile.is_open();

    return isOwner_;
  }

  std::string fileName_;
  bool isOwner_;
};


BOOST_FIXTURE_TEST_SUITE( TextConfigurationParser, TextConfigurationParserFixture)

  BOOST_AUTO_TEST_CASE(nonExistentFile_readByLine_throw)
  {
    runnerd::common::TextConfigurationParser textConfigurationParser(fileName_);

    BOOST_REQUIRE_THROW(textConfigurationParser.readByLine(), runnerd::core::BaseException);
  }

  BOOST_AUTO_TEST_CASE(existentFile_readByLine_no_throw)
  {
    createFile();

    runnerd::common::TextConfigurationParser textConfigurationParser(fileName_);
    BOOST_REQUIRE_NO_THROW(textConfigurationParser.readByLine());
  }

BOOST_AUTO_TEST_SUITE_END()



