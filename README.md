# Personal HTTP Server in Modern C++

Welcome to my personal HTTP Server project, built with modern C++17, featuring concurrency, asynchronous I/O, CGI support, and simple HTTP routing.

## Features

- Handles HTTP GET and POST requests
- Simple CGI support to handle POST data
- Serves static HTML content
- Lightweight and easy to extend
- Written using low-level POSIX sockets

## Getting Started

### Prerequisites

- A C++17 compatible compiler (e.g., g++ 7+)
- Linux or macOS environment (POSIX sockets)

### Building

Clone the repo and run:

```bash
g++ -std=c++17 -pthread main.cpp server.cpp -o server

```
## Running

Run the server executable:

```bash
./server
```
## Test your server
- Open browser → go to http://localhost:8080/
- Test POST request (CGI) with curl:
  
```bash
  curl -d "name=Riya&field=Electronics" -X POST http://localhost:8080/cgi-bin/demo
```
  You should see a response like:
  
```bash
  CGI response with input: name=Riya&field=Electronics
```
<img width="350" alt="Screenshot 2025-05-25 at 1 44 42 PM" src="https://github.com/user-attachments/assets/44fb89f6-94b0-4a18-bb41-5025e88738eb" />
  


