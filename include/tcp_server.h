#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>

// Define a class for the TCP TCP server
class TcpServer {
  public:
    // Constructor to initialize the server with the port
    TcpServer(int port);

    // Destructor to clean up resources
    ~TcpServer();

    // Method to start the server and begin accepting connections
    void start();

    // Method to stop the server and close connections
    void stop();

  private:
    int server_fd;    // Socket file
    int port;         // Port number to listen on
    bool is_running;  // Whether the server is is running 

    // Internal method to set up the server (e.g. create socket, bind, etc.)
    bool setupServer();

    // Internal method to accept incoming connections and handle communication
    void handleConnections();

    // Internal method to handle incoming data from a client
    void handleClient(int client_socket);

    // Utility method to log messages
    void log(const std::string& message);
};

#endif // TCP_SERVER_H
