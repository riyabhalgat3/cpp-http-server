#pragma once
#include <string>
#include <functional>
#include <unordered_map>

struct HTTPRequest {
    std::string method;
    std::string path;
    std::string body;
};

struct HTTPResponse {
    int status_code;
    std::string content_type;
    std::string body;

    static HTTPResponse from_file(const std::string& filename);
};

class HTTPServer {
public:
    HTTPServer() = default;

    void run(uint16_t port);

    void route(const std::string& path, std::function<HTTPResponse(const HTTPRequest&)> handler);

private:
    std::unordered_map<std::string, std::function<HTTPResponse(const HTTPRequest&)>> routes_;

    HTTPResponse handle_request(const HTTPRequest& request);
};

