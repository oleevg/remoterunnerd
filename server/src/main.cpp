/*
 * main.cpp
 *
 *  Created on: 10/1/17
 *      Author: Oleg F., fedorov.ftf@gmail.com
 */

#include <core/ulog.h>

#include "Application.hpp"

namespace runnerd {

  class ULogWrapper {
    public:
      ULogWrapper()
      {
        ulog_init();
      }

      ~ULogWrapper()
      {
        ulog_clean();
      }
  };

}

int main(int argc, const char* argv[])
{
  runnerd::ULogWrapper ulog;

  runnerd::server::Application app;

  return app.run(argc, argv);
}

