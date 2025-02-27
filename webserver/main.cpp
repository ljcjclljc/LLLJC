
#include <iostream>
#include"./mysql/mysql_connect.h"
#include"./mysql/mysql_connect_poll.h"
#include"./ser/beast_server.h"
#include"./ser/http_server.h"
#include"./thread/threadpool.h"
int id;

using namespace std;
unsigned short port = 8080;
threadpool thr(420);


int main()
{
    //压力测试
//     clock_t begin = clock();
//     cout<<"begin"<<endl;
//     mysql_connect_poll* cp = mysql_connect_poll::get_instance();
//     for (int i = 0; i < 10000; ++i) {
//         mysql_connect conn;
//         char sql[1024] = { 0 };
//         sprintf(sql, "insert into user(username,passwd) values('%s','%s')"
//             , "zhangsan","male");
//         conn.init("127.0.0.1", 3306, "root", "123456", "yourdb");
//         conn.update(sql);
//         cout<<"ok"<<' '<<i<<endl;

// // #if 1
// //         shared_ptr<mysql_connect> sp = cp->get_connect();
// //         char sql[1024] = { 0 };
// //         sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')"
// //             , "zhangsan", 20, "male");
// //         sp->update(sql);
// // #endif
//     }
//     clock_t end = clock();
//     cout << (end - begin) << "ms" << endl;

//测试beast_server
try {
    http_server server(PORT,thr);
    server.run();
    getchar(); // 阻塞
} catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
return 0;

    return 0;
}
