cmake_minimum_required(VERSION 3.10)
project(TcpServer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate Google Test
find_package(GTest REQUIRED)

# Add the executable for the main server
add_executable(tcp_server src/main.cpp src/tcp_server.cpp)

# Link necessary libraries
target_link_libraries(tcp_server pthread)

# Add the test executable
add_executable(test_tcp_server test/test_tcp_server.cpp src/tcp_server.cpp)

# Link Google Test and pthread to the test executable
target_link_libraries(test_tcp_server GTest::gtest_main)

# Enable testing and add the test
enable_testing()
add_test(NAME TcpServerTest COMMAND test_tcp_server)

