add_test([=[FullScenarioTest2.FullScenario]=]  /Users/viktor/MIPT/Projects/SwiftMessage/SwiftMessage/build-tests/integration_tests/SwiftMessage_Tests [==[--gtest_filter=FullScenarioTest2.FullScenario]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[FullScenarioTest2.FullScenario]=]  PROPERTIES WORKING_DIRECTORY /Users/viktor/MIPT/Projects/SwiftMessage/SwiftMessage/build-tests/integration_tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  SwiftMessage_Tests_TESTS FullScenarioTest2.FullScenario)
