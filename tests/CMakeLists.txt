include(FetchContent)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(GTest REQUIRED)

add_library(GTest::Main ALIAS gtest_main)
