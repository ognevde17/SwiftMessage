//
// Created by sheyme on 28/04/25.
//

#ifndef SWIFTMESSAGE_TESTS_SCREEN_HANDLER_UTILS_HPP_
#define SWIFTMESSAGE_TESTS_SCREEN_HANDLER_UTILS_HPP_

#include <string>
#include <random>
#include <ctime>

#include "../../include/screen_handler/interface.hpp"


namespace Utils {
std::string GenerateRandomString(size_t length = 10) {
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  static std::mt19937 gen(std::time(nullptr));
  std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);
  std::string string;
  string.reserve(length);
  for (size_t idx = 0; idx < length; ++idx) {
    string += alphanum[dis(gen)];
  }
  return string;
}

std::vector<Message> GenerateRandomMessages(size_t count) {
  std::vector<Message> vector;
  for (size_t idx = 0; idx < count; ++idx) {
    size_t str_length = 5 + (std::rand() % 16);
    std::string text = GenerateRandomString(str_length);
    ColorPairs type = static_cast<ColorPairs>(std::rand() % 3 + 1);
    vector.emplace_back(std::move(text), type);
  }
  return vector;
}
}

#endif  // SWIFTMESSAGE_TESTS_SCREEN_HANDLER_UTILS_HPP_
