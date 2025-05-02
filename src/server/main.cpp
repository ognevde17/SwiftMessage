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

  auto thread_func = [&server]() {
    server.Run();
    std::cout << "Server is running" << std::endl;
  };

  std::thread server_thread(thread_func);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  server.Stop();
  server_thread.join();
  std::cout << "Server is stopped" << std::endl;
  return 0;
}