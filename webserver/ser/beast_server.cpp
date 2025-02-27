#include "beast_server.h"
void http_session::send_response(http::status status, const std::string& body) {
    auto selfs(shared_from_this());
    res.version(request_.version());
    res.result(status);
    res.set(http::field::content_type, "text/html");
    res.content_length(body.size());
    res.body() = body;
    res.prepare_payload();
    std::cout<<res.body()<<'\n';
    if(socket_.is_open()){
        std::cout<<"socket is open"<<'\n';
    }else{
        std::cout<<"socket is not open"<<'\n';
    }
    http::async_write(socket_, res,
        [this, selfs](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);     // 忽略未使用变量
            if (ec) return;
            selfs->socket_.shutdown(tcp::socket::shutdown_send, ec);  // 关闭连接
            if (ec) return;
            selfs->socket_.close(ec);  // 关闭套接字
        });
}

void http_session::handle_request() {
    std::cout << "Request Method: " << request_.method() << "\n";  // 输出请求方法
std::cout << "Request Target: " << request_.target() << "\n";  // 输出请求目标
std::cout << "HTTP Version: " << request_.version() << "\n";   // 输出HTTP版本
   std::string filename = request_.target().to_string();
   if (filename == "/") filename = "/index.html";  // 默认首页

   std::string filepath = "www" + filename; // 假设文件在 www 目录下

   std::ifstream ifs(filepath);
   if (!ifs) {
       // 文件未找到
       send_response(http::status::not_found, "File not found");
       return;
   }

   std::string file_content((std::istreambuf_iterator<char>(ifs)),
                             std::istreambuf_iterator<char>());

   // 发送成功响应
//    if(sum%2==1){
//     file_content = "hello world111111";
//    }else{
//     file_content = "hello world222222";
//    }
   send_response(http::status::ok, file_content);
}

void http_session::do_read() {
    auto self(shared_from_this());
    http::async_read(socket_, buffer_, request_,
        [this, self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (ec) return;
            handle_request();
        });
}