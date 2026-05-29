#include "fs_utils.h"
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>

bool create_dir(const std::string& path)
{
    return mkdir(path.c_str(), 0755) == 0;
}

bool write_file(const std::string& path, const std::string& content)
{
    FILE* f = fopen(path.c_str(), "w");
    if (!f) return false;
    fwrite(content.data(), 1, content.size(), f);
    fclose(f);
    return true;
}

bool file_exists(const std::string& path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

bool is_dir(const std::string& path)
{
    struct stat st;
    stat(path.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

std::vector<std::string> list_dir(const std::string& dir)
{
    std::vector<std::string> files;
    DIR* dp = opendir(dir.c_str());
    if (!dp) return files;

    dirent* ent;
    while ((ent = readdir(dp)))
    {
        std::string name = ent->d_name;
        if (name == "." || name == "..") continue;
        files.push_back(name);
    }
    closedir(dp);
    return files;
}

void init_server_env()
{
    if (!file_exists("conf")) create_dir("conf");
    if (!file_exists("www")) create_dir("www");

    std::string conf = "# PASS-Server default config\nlisten 8080;\nroot \"./www\";\nautoindex on;\n";
    if (!file_exists("conf/server.conf"))
        write_file("conf/server.conf", conf);

    std::string index = "<html><head><meta charset=\"utf-8\"><title>PASS-Server</title></head><body><h1>Welcome to PASS-Server</h1><p>Static HTML Web Server</p></body></html>";
    if (!file_exists("www/index.html"))
        write_file("www/index.html", index);
}
