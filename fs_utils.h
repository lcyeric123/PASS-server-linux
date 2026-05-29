#ifndef FS_UTILS_H
#define FS_UTILS_H

#include <string>
#include <vector>

bool create_dir(const std::string& path);
bool write_file(const std::string& path, const std::string& content);
bool file_exists(const std::string& path);
bool is_dir(const std::string& path);
std::vector<std::string> list_dir(const std::string& dir);
void init_server_env();

#endif
