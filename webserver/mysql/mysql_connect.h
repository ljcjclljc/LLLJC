#pragma once

#include<string>
#include<mysql/mysql.h>
#include<list>
#include<mutex>
#include<ctime>
#include<cstdlib>

using namespace std;

class mysql_connect
{
    public:
    //初始化函数
    mysql_connect();
    //析构函数
    ~mysql_connect();

    // 连接数据库
    bool init(string ip,unsigned short port,string user,string password,string database);

    //更新操作
    bool update(string sql);

    //查询操作
    MYSQL_RES* query(string sql);

    //刷新空闲连接时间点
    void refresh_conn(){
        m_alive_time = clock();
    };

    //返回存活时间
    clock_t get_alive_time(){
        return clock() - m_alive_time;
    };

    private:
    
    MYSQL* m_conn;//连接
    clock_t m_alive_time;//存活时间


};