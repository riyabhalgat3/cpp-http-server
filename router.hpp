#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <sstream>
#include <fstream>

struct HTTPRequest {
    std::string method;
    std::string path;

    static HTTPRequest parse(const std::string& raw) {
        std::istringstream stream(raw);
        HTTPRequest req;
        stream >> req.method >> req.path;
        return req;
    }
};

struct HTTPResponse {
    int status_code;
    std::string content_type;
    std::string body;

    static HTTPResponse from_file(const std::string& path) {
        std::ifstream file(path);
        if (!file) return {404, "text/plain", "File not found"};
        std::ostringstream ss;
        ss << file.rdbuf();
        return {200, "text/html", ss.str()};
    }

    std::string to_string() const {
        std::ostringstream res;
        res << "HTTP/1.1 " << status_code << " OK\r\n";
        res << "Content-Type: " << content_type << "\r\n";
        res << "Content-Length: " << body.size() << "\r\n\r\n";
        res << body;
        return res.str();
    }
};

class Router {
    std::unordered_map<std::string, std::function<HTTPResponse(const HTTPRequest&)>> routes;

public:
    void add_route(const std::string& path, const std::function<HTTPResponse(const HTTPRequest&)>& handler) {
        routes[path] = handler;
    }

    HTTPResponse handle(const HTTPRequest& req) const {
        auto it = routes.find(req.path);
        if (it != routes.end()) return it->second(req);
        return {404, "text/plain", "Not Found"};
    }
};
