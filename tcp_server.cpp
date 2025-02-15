#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 7777;

int main() {
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};

	// Create socket file descriptor
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {
		std::cerr << "Socket creation failed\n";
		return -1;
	}

	// Attach socket to the port 7777
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Bind the socket to the network address and port
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		std::cerr << "Bind failed\n";
		return -1;
	}

	std::cout << "Server started on port " << PORT << "\n";

	// Listen for incoming connections
	listen(server_fd, 3);

	while (true) {
		std::cout << "Waiting for a connection...\n";

		new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		if (new_socket < 0) {
			std::cerr << "Accept failed\n";
			return -1;
		}

		std::cout << "Connection established!\n";

		// Read data continuously from client
		while (true) {
			memset(buffer, 0, sizeof(buffer)); // Clear buffer
			int bytesRead = read(new_socket, buffer, sizeof(buffer) - 1);

			if (bytesRead <= 0) {
				std::cout << "Connection ended\n";
				close(new_socket);
				break;
			}

			std::cout << "Received: " << buffer << "\n";

			// Echo back the message
			send(new_socket, buffer, strlen(buffer), 0);
		}
	}

	return 0;
}
