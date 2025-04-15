#include <atomic>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>
#include <utility>

#include "../../include/server/server.hpp"

int main() {
  Server& server = Server::GetInstance();

  server.Run();

  return 0;
}