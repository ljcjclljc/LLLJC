#pragma once

#include"public.h"

using namespace std;

class mysql_connect_poll
{
    public:
    //单例模式获取连接池对象
    static mysql_connect_poll* get_instance();
    shared_ptr<mysql_connect> get_connect();

    private:
    //单例模式私有化构造函数
    mysql_connect_poll();
    ~mysql_connect_poll(){};
    //运行在独立的线程中，不断生产连接
    void produce_connect();
    //扫描空闲连接，超时的连接进行回收
    void scan_free_connect();

    string m_ip;
    unsigned int m_port;
    string m_user;
    string m_password;
    string m_database;

    int m_max_conn;//最大连接数
    int m_init_conn;//初始化连接数
    int m_max_idle_time;//最大空闲时间
    int m_connect_timeout;//连接超时时间

    queue<mysql_connect*> mysql_connect_queue;//连接队列

    mutex m_mutex;//互斥锁

    atomic_int m_connCnt;//空闲连接数

    condition_variable m_cond;//条件变量



};