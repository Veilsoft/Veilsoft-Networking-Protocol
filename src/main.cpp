#include "../include/tcp_server.h"

int main() {
  TcpServer server(7777);
  server.start();
  server.stop();
  return 0;
}
