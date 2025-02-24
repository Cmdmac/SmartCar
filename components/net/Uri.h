#ifndef _URI_H_
#define _URI_H_

#include <iostream>
#include <string>
#include <vector>

namespace Stark {
    class Uri {
    private:
        std::string uriString;
        std::string scheme;
        std::string host;
        int port;
        std::string path;
        std::vector<std::string> querys;
        // std::vector<std::pair<std::string, std::string>> querys;

        std::string tolower(std::string &s) {
            std::string str = s;
            for (size_t i = 0; i < str.length(); ++i) {
                char c = str[i];
                if (c >= 'A' && c <= 'Z') {
                    c += 32;
                    str[i] = c;
                }
            }
            // std::cout << str << std::endl;
            return str;
        }

        // 辅助函数：分割字符串
        void splitString(const std::string& str, char delimiter, std::vector<std::string>& tokens) {
            size_t start = 0;
            size_t end = str.find(delimiter);
            while (end!= std::string::npos) {
                tokens.push_back(str.substr(start, end - start));
                start = end + 1;
                end = str.find(delimiter, start);
            }
            tokens.push_back(str.substr(start));
        }

        // 解析查询参数
        void parseQueryParams() {
            size_t queryStart = uriString.find('?');
            if (queryStart!= std::string::npos) {
                std::string queryString = uriString.substr(queryStart + 1);
                splitString(queryString, '&', querys);
            }
        }

        // 解析URI的各个部分
        void parseUri() {
            size_t schemeEnd = uriString.find("://");
            if (schemeEnd!= std::string::npos) {
                scheme = uriString.substr(0, schemeEnd);
                size_t hostStart = schemeEnd + 3;
                size_t pathStart = uriString.find('/', hostStart);
                if (pathStart!= std::string::npos) {
                    host = uriString.substr(hostStart, pathStart - hostStart);
                    path = uriString.substr(pathStart);
                } else {
                    host = uriString.substr(hostStart);
                    path = "/";
                }

                size_t portStart = host.find(':');
                if (portStart!= std::string::npos) {
                    port = std::stoi(host.substr(portStart + 1));
                    host = tolower(host);
                    host = host.substr(0, portStart);
                } else {
                    if (scheme == "http") {
                        port = 80;
                    } else if (scheme == "https") {
                        port = 443;
                    } else {
                        port = -1; // 表示未指定常见协议对应的默认端口
                    }
                }

                parseQueryParams();
            } else {
                // 这里可以根据需求进行更复杂的处理，比如抛出异常或设置默认值
                // 暂时简单处理，假设是相对路径
                path = uriString;
                port = -1;
                host = "";
                scheme = "";
            }
        }

    public:
        Uri(const char* uri);
        Uri(const char* scheme, char* host, int port);
        Uri(const char* scheme, const char*, int port);
        void appendPath(std::string path);
        void appendQuery(std::string key, std::string value);
        void appendQuery(std::string key, int value);

        std::string getScheme() const;
        std::string getHost() const;
        int getPort() const;
        std::string getPath() const;
        const std::vector<std::string>& getQueryParams() const;
        void printUriDetails() const;
        std::string toString();


    };

    class UriBuilder {
        public:
            

        private:
            std::string host;
            int port;
            std::vector<std::string> paths;
            std::vector<std::pair<std::string, std::string>> querys;
    };
}
#endif