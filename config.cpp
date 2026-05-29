#include "config.h"
#include <fstream>
#include <sstream>

ServerConfig load_config(const std::string& path)
{
    ServerConfig cfg;
    std::ifstream f(path);
    if (!f.is_open()) return cfg;

    std::string line;
    while (std::getline(f, line))
    {
        std::istringstream iss(line);
        std::string key, val;
        iss >> key >> val;

        if (key == "listen")
        {
            try { cfg.port = std::stoi(val); } catch (...) {}
        }
        else if (key == "root")
            cfg.root = val;
        else if (key == "autoindex" && val == "on")
            cfg.autoindex = true;
    }
    return cfg;
}
