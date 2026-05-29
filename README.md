markdown
  
# PASS‑Server
轻量级原生 C++ Linux 静态网页服务器，无第三方依赖
Lightweight native C++ static web server for Linux, zero third‑party dependencies.

## 概述 / Overview
PASS‑Server 是一款使用纯 C++ 编写的简易静态文件服务器。程序首次运行时会自动生成运行目录与配置文件，可正确提供 HTML、CSS、JavaScript 及各类图片资源访问服务。
PASS‑Server is a simple static file server written in pure C++. It automatically generates runtime directories and configuration files on the first launch, serving HTML, CSS, JavaScript and image files correctly.

## 功能特性 / Features
- 自动创建 conf、www 目录与默认配置文件
  Auto‑create conf/, www/ directories and default configuration
- 自动生成默认 index.html 欢迎首页
  Auto‑generate default index.html welcome page
- 支持静态资源：HTML、CSS、JS、PNG、JPG、GIF
  Serve static resources: HTML, CSS, JS, PNG, JPG, GIF
- 开启目录自动浏览功能（autoindex）
  Auto directory listing (autoindex)
- 标准 HTTP 错误页面：403 禁止访问、404 页面不存在
  Standard HTTP error pages: 403 Forbidden, 404 Not Found
- 自动识别文件 MIME 类型，保证浏览器正常渲染
  Correct MIME‑type recognition for all supported file types
- 单可执行文件，部署简单便捷
  Single executable file, easy to deploy

## 编译命令 / Compile Command
```bash
g++ main.cpp pass_socket.cpp fs_utils.cpp config.cpp -o PASS‑Server
 
 
运行程序 / Run
 
bash
  
chmod +x ./PASS‑Server
./PASS‑Server
 
 
访问地址 / Access
 
浏览器打开以下地址访问网页
Open in browser:
 
plaintext
  
http://127.0.0.1:8080
 
 
自动生成目录结构 / Auto‑Generated Structure
 
plaintext
  
./
├── PASS‑Server          # 主程序 Main program
├── conf/
│   └── server.conf      # 配置文件 Configuration file
└── www/
    └── index.html       # 默认首页 Default homepage
 
 
配置说明（conf/server.conf）/ Configuration
 
plaintext
  
listen 8080;
root "./www";
autoindex on;
 
 
- listen：监听端口号 Listening port number
- root：网站根目录 Website root directory
- autoindex on/off：开启/关闭目录浏览 Enable or disable directory listing
 
支持文件类型 / Supported File Types
 
- .html / .htm
- .css
- .js
- .png
- .jpg / .jpeg
- .gif
 
