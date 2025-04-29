#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "../include/client/client.hpp"
#include "../include/common/messages.hpp"
#include "../include/server/server.hpp"

class ServerThread {
 public:
  ServerThread() : running_(false) {}

  void Start() {
    if (running_.load()) return;

    running_.store(true);
    thread_ = std::thread([this] {
      try {
        Server::GetInstance().Run();
      } catch (const std::exception& ex) {
        std::cerr << "[ServerThread] exception: " << ex.what() << std::endl;
      }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  void Stop() {
    if (!running_.load()) return;

    running_.store(false);
    Server::GetInstance().Stop();

    if (thread_.joinable()) thread_.join();
  }

  ~ServerThread() { Stop(); }

 private:
  std::thread thread_;
  std::atomic<bool> running_;
};

// TEST(ServerTest, InitializeServer) {
//   ServerThread server_thread;
//   server_thread.Start();

//   std::this_thread::sleep_for(std::chrono::seconds(100));

//   server_thread.Stop();
// }

TEST(FullScenarioTest2, FullScenario) { EXPECT_TRUE(2 == 2); }

// Точка входа для GTest
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
