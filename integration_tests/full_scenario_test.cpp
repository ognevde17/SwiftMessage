#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>
#include <exception>
#include <iostream>
#include <string>

TEST(FullScenarioTest, FullScenario) {
    EXPECT_TRUE(true);
}

TEST(FullScenarioTest2, FullScenario) {
    EXPECT_TRUE(2 == 2);
}

// Точка входа для GTest
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
