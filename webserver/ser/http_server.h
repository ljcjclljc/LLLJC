#include"beast_server.h"
#include"../thread/threadpool.h"
class http_server {
    net::io_context io_context_;
    tcp::acceptor acceptor_;

public:
    http_server(short port,threadpool& thr) : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)),pool(thr) {
        do_accept();
    }

    void run() {
        io_context_.run();
    }

private:
    void do_accept() ;
    threadpool &pool;
    //pool.enqueue<void>(http_server::do_accept()::<lambda(boost::beast::error_code, boost::asio::ip::tcp::socket)>::<lambda()>());
};
