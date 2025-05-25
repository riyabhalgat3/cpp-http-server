#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const int PORT = 8080;

std::string html_page = R"(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Riya's Personal HTTP Server</title>
  <style>
    body { 
      background-color: #f9f9f9; 
      font-family: Arial, sans-serif; 
      text-align:center; 
      margin-top:50px; 
      display: flex; 
      flex-direction: column; 
      min-height: 100vh;
    }
    main {
      flex: 1;
    }
    footer {
      margin-top: auto;
      padding: 10px 0;
      font-size: 0.9rem;
      color: #666;
    }
  </style>
</head>
<body>
  <main>
    <h1>Hi, I'm Riya 👋</h1>
    <h2>Electronics & Communication Engineer | C++ Dev</h2>
    <p>Welcome to my personal HTTP server, built using modern C++ techniques including concurrency, asynchronous I/O, and CGI support.</p>
  </main>
  <footer>© 2025 Riya Bhalgat</footer>
</body>
</html>
)";

void handle_client(int client_fd) {
    char buffer[4096];
    ssize_t received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (received <= 0) {
        close(client_fd);
        return;
    }
    buffer[received] = 0;

    std::string request(buffer);

    std::istringstream req_stream(request);
    std::string method, path, version;
    req_stream >> method >> path >> version;

    std::string response;

    if (method == "GET" && path == "/") {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
                   std::to_string(html_page.size()) + "\r\n\r\n" + html_page;
    } else if (method == "POST" && path == "/cgi-bin/demo") {
        size_t pos = request.find("\r\n\r\n");
        std::string body = "";
        if (pos != std::string::npos) {
            body = request.substr(pos + 4);
        }
        std::string body_response = "CGI response with input: " + body;
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " +
                   std::to_string(body_response.size()) + "\r\n\r\n" + body_response;
    } else {
        std::string not_found = "404 Not Found";
        response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: " +
                   std::to_string(not_found.size()) + "\r\n\r\n" + not_found;
    }

    send(client_fd, response.c_str(), response.size(), 0);
    close(client_fd);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        handle_client(client_fd);
    }

    close(server_fd);
    return 0;
}

