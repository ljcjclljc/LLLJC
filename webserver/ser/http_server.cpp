#include"http_server.h"
//#include"../thread/threadpool.h"
void http_server::do_accept() {
    acceptor_.async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::cout << "Accepting...";
                // std::make_shared<http_session>(std::move(socket))->run();
                // std::thread t([this,&socket]() {
                //     std::make_shared<http_session>(std::move(socket))->run();

                // });
                // t.join();
                auto func=std::function<void()>([this,&socket]()mutable{
                    std::make_shared<http_session>(std::move(socket))->run();
                });
                pool.enqueue(func);
            }else
            {
                std::cout << ec.message() << std::endl;
            }
            do_accept(); // 继续接受下一个连接
        });
}