#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/beast.hpp>
#include <memory>
#define PORT 8080
namespace beast = boost::beast;        // 从 Boost.Beast 获取所有内容
namespace http = beast::http;          // 从 Boost.Beast HTTP 获取所有内容
namespace net = boost::asio;           // 从 Boost.Asio 获取所有内容
using tcp = net::ip::tcp;              // 从 net 中定义 TCP 类型

// 用于存储握手后的连接
class http_session : public std::enable_shared_from_this<http_session> {
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> request_;
    http::response<http::string_body> res;
    int sum;

public:
    // 构造器
    http_session(tcp::socket socket ): socket_(std::move(socket)){
        
    }

    // 开始会话
    void run() {
        do_read();
    }

private:
    // 读取请求
    void do_read();

    // 处理请求
    void handle_request();

    // 发送响应
    void send_response(http::status status, const std::string& body);
};
