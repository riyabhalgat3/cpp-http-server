#include "server.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <cstdio>
#include <memory>
#include <array>

HTTPResponse HTTPResponse::from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {404, "text/plain", "File not found"};
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return {200, "text/html", buffer.str()};
}

void HTTPServer::route(const std::string& path, std::function<HTTPResponse(const HTTPRequest&)> handler) {
    routes_[path] = handler;
}

// Simulate CGI execution: run a shell command or script and capture output
std::string run_cgi_script(const std::string& script_input) {
    // Example: echo input back - replace with actual CGI script execution
    std::string command = "echo 'CGI response with input: " + script_input + "'";
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) return "Failed to run CGI script.";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

HTTPResponse HTTPServer::handle_request(const HTTPRequest& request) {
    // Check if route matches exactly
    auto it = routes_.find(request.path);
    if (it != routes_.end()) {
        return it->second(request);
    }

    // Basic CGI path handling
    if (request.path.rfind("/cgi-bin/", 0) == 0) { // starts with /cgi-bin/
        if (request.method == "POST") {
            std::string cgi_result = run_cgi_script(request.body);
            return {200, "text/html", cgi_result};
        } else {
            return {405, "text/plain", "Method Not Allowed"};
        }
    }

    return {404, "text/plain", "Not Found"};
}

// Dummy simplified server run loop (replace with real socket server in your project)
void HTTPServer::run(uint16_t port) {
    std::cout << "Server listening on port " << port << std::endl;
    // Here you would accept connections, parse HTTP requests,
    // and call handle_request then send response.
    // For demo, we'll simulate a POST request to /cgi-bin/script

    HTTPRequest demo_req;
    demo_req.method = "POST";
    demo_req.path = "/cgi-bin/test";
    demo_req.body = "name=Riya&field=Electronics";

    HTTPResponse resp = handle_request(demo_req);
    std::cout << "Demo CGI POST Response:\n" << resp.body << std::endl;

    // Your actual server loop implementation needed here.
}

