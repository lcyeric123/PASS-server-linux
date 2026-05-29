#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct ServerConfig
{
    int port = 8080;
    std::string root = "./www";
    bool autoindex = true;
};

ServerConfig load_config(const std::string& path);

#endif
