#include <gtest/gtest.h>
#include "../include/tcp_server.h"

// Test the constructor
TEST(TcpServerTest, ConstructorTest) {
  TcpServer server(7777);
}

