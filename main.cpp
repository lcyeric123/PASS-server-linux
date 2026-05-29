#include "pass_socket.h"
#include "fs_utils.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <sys/socket.h>

std::string get_request_path(const std::string& req)
{
    std::istringstream iss(req);
    std::string method, path, proto;
    iss >> method >> path >> proto;
    return path;
}

// 严格修复 MIME 类型
std::string get_mime_type(const std::string& path)
{
    if (path.rfind(".html") != std::string::npos || path.rfind(".htm") != std::string::npos)
        return "text/html; charset=utf-8";
    if (path.rfind(".css") != std::string::npos)
        return "text/css; charset=utf-8";
    if (path.rfind(".js") != std::string::npos)
        return "application/javascript; charset=utf-8";
    if (path.rfind(".png") != std::string::npos)
        return "image/png";
    if (path.rfind(".jpg") != std::string::npos || path.rfind(".jpeg") != std::string::npos)
        return "image/jpeg";
    if (path.rfind(".gif") != std::string::npos)
        return "image/gif";
    // 访问根目录默认返回 html
    return "text/html; charset=utf-8";
}

std::string make_error_page(int code)
{
    std::string msg;
    if (code == 403) msg = "403 Forbidden";
    else if (code == 404) msg = "404 Not Found";
    else msg = std::to_string(code);
    return "<html><head><meta charset='utf-8'></head><body><h1>" + msg + "</h1></body></html>";
}

std::string make_dir_list(const std::string& root, const std::string& url_path)
{
    std::string dir = root + url_path;
    auto files = list_dir(dir);

    std::string html = "<html><head><meta charset='utf-8'><title>Directory " + url_path + "</title></head><body>";
    html += "<h3>Index of " + url_path + "</h3><hr><ul>";
    for (auto& f : files)
    {
        html += "<li><a href=\"" + url_path + f + "\">" + f + "</a></li>";
    }
    html += "</ul></body></html>";
    return html;
}

std::string read_content(const std::string& root, const std::string& path, bool autoindex, int& out_code, std::string& mime_out)
{
    std::string file_path;

    // 根目录强制指向 index.html
    if (path == "/")
    {
        file_path = root + "/index.html";
        mime_out = "text/html; charset=utf-8";
    }
    else
    {
        file_path = root + path;
        mime_out = get_mime_type(path);
    }

    std::string dir_check = root + path;
    if (is_dir(dir_check))
    {
        std::string index = dir_check + "/index.html";
        if (file_exists(index))
        {
            std::ifstream f(index);
            out_code = 200;
            mime_out = "text/html; charset=utf-8";
            return std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        }
        else if (autoindex)
        {
            out_code = 200;
            mime_out = "text/html; charset=utf-8";
            return make_dir_list(root, path);
        }
        else
        {
            out_code = 403;
            mime_out = "text/html; charset=utf-8";
            return make_error_page(403);
        }
    }

    if (!file_exists(file_path))
    {
        out_code = 404;
        mime_out = "text/html; charset=utf-8";
        return make_error_page(404);
    }

    std::ifstream f(file_path, std::ios::binary);
    out_code = 200;
    mime_out = get_mime_type(path);
    return std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}

std::string build_response(int code, const std::string& body, const std::string& mime)
{
    return "HTTP/1.1 " + std::to_string(code) + " OK\r\n"
           "Content-Type: " + mime + "\r\n"
           "Content-Length: " + std::to_string(body.size()) + "\r\n"
           "Connection: close\r\n\r\n" + body;
}

int main()
{
    init_server_env();
    auto cfg = load_config("conf/server.conf");
    cfg.root = "./www";

    if (!pass::socket_init())
    {
        std::cerr << "Network init failed\n";
        return 1;
    }

    sock_t server = pass::create_tcp_socket();
    if (server == invalid_sock)
    {
        std::cerr << "Create socket failed\n";
        pass::socket_cleanup();
        return 1;
    }

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (!pass::bind_socket(server, cfg.port) || !pass::listen_socket(server))
    {
        std::cerr << "Bind/Listen failed\n";
        pass::close_socket(server);
        return 1;
    }

    std::cout << "PASS‑Server Linux started\nPort: " << cfg.port << "\nRoot: " << cfg.root << "\n";

    while (true)
    {
        sock_t client = pass::accept_socket(server);
        if (client == invalid_sock) continue;

        std::string req = pass::recv_data(client, 2048);
        std::string path = get_request_path(req);

        int code = 200;
        std::string mime;
        std::string body = read_content(cfg.root, path, cfg.autoindex, code, mime);
        std::string resp = build_response(code, body, mime);

        pass::send_data(client, resp);
        pass::close_socket(client);
    }

    pass::close_socket(server);
    return 0;
}
