//
// Created by sheyme on 28/04/25.
//

#ifndef SWIFTMESSAGE_TESTS_SCREEN_HANDLER_UTILS_HPP_
#define SWIFTMESSAGE_TESTS_SCREEN_HANDLER_UTILS_HPP_

#include <string>
#include <random>
#include <ctime>

#include "../../include/screen_handler/interface.hpp"

namespace testing_data {
const int kMessagesCount = 40;
const size_t kBaseMessageLength = 10;
}

namespace utils {
std::string GenerateRandomString(
    size_t length = testing_data::kBaseMessageLength) {
  static const char kAlphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  static std::mt19937 gen(std::time(nullptr));
  std::uniform_int_distribution<> dis(0, sizeof(kAlphanum) - 2);
  std::string string;
  string.reserve(length);
  for (size_t idx = 0; idx < length; ++idx) {
    string += kAlphanum[dis(gen)];
  }
  return string;
}

std::vector<Message> GenerateRandomMessages(size_t count) {
  std::vector<Message> vector;
  for (size_t idx = 0; idx < count; ++idx) {
    size_t str_length = testing_data::kBaseMessageLength + (std::rand() % 11);
    std::string text = GenerateRandomString(str_length);
    ColorPairs type = static_cast<ColorPairs>((std::rand() % 3) + 1);
    vector.emplace_back(std::move(text), type);
  }
  return vector;
}
}  // namespace utils

#endif  // SWIFTMESSAGE_TESTS_SCREEN_HANDLER_UTILS_HPP_
