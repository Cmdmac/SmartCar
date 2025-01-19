#include "Uri.h"
#include <sstream>
using namespace Stark;

// 构造函数
Uri::Uri(const char* uri) : uriString(uri) {
    parseUri();
}

Uri::Uri(const char* scheme, const char* host, int port = -1) : scheme(scheme), host(host), port(port) {

}

// 获取协议
std::string Uri::getScheme() const {
    return scheme;
}

// 获取主机名
std::string Uri::getHost() const {
    return host;
}

// 获取端口号
int Uri::getPort() const {
    return port;
}

// 获取路径
std::string Uri::getPath() const {
    return path;
}

// 获取查询参数（以向量形式返回）
const std::vector<std::string>& Uri::getQueryParams() const {
    return querys;
}

void Uri::appendPath(std::string path) {
    this->path.append(path);
}

void Uri::appendQuery(std::string key, std::string value) {
    std::string s;
    s.append(key).append("=").append(value);
    querys.push_back(s);
}

void Uri::appendQuery(std::string key, int value) {
    std::string s;
    s.append(key).append("=");
    std::stringstream ss;
    ss << value;
    s.append(ss.str());
    querys.push_back(s);
}

// 打印URI的详细信息
void Uri::printUriDetails() const {
    std::cout << "Scheme: " << getScheme() << std::endl;
    std::cout << "Host: " << getHost() << std::endl;
    std::cout << "Port: " << getPort() << std::endl;
    std::cout << "Path: " << getPath() << std::endl;
    std::cout << "Query Parameters: " << std::endl;
    for (const auto& param : querys) {
        std::cout << "    " << param << std::endl;
    }
}

std::string Uri::toString() {
    std::stringstream ss;
    ss << scheme << "://" << host;
    if (port != -1) {
        ss << ":" << port;
    }
    ss << path;
    if (querys.size() > 0) {
        ss << "?";
        for (size_t i = 0; i < querys.size(); i++) {
            ss << querys[i];
            if (i < querys.size() - 1) {
                ss << "&";
            }
        }
    }
    return ss.str();
}

