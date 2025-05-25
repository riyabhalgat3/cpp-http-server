# C++ HTTP Server

A simple multi-threaded HTTP server in C++ supporting GET and POST requests, CGI, and concurrency.

## Features

- Basic HTTP/1.1 server
- Supports GET and POST requests
- Handles simple CGI POST input
- Multi-threaded request handling
- Written using standard C++17 and POSIX sockets

## How to Build and Run

```bash
g++ -std=c++17 -pthread main.cpp server.cpp -o server
./server

Navigate to http://localhost:8080 in your browser.


Add, commit, and push:

```bash
git add README.md
git commit -m "Add README with project description"
git push
