
#include"public.h"
#include"mysql_connect_poll.h"
using namespace std;

//单例模式获取连接池对象
mysql_connect_poll* mysql_connect_poll::get_instance()
{
    static mysql_connect_poll mysql_connect_poll;
    return &mysql_connect_poll;
}
//给出连接，从连接池获取一个可用的空闲连接
shared_ptr<mysql_connect> mysql_connect_poll::get_connect(){
    unique_lock<mutex> lock(m_mutex);
    while(mysql_connect_queue.empty()){//连接池为空
if(std::cv_status::timeout==m_cond.wait_for(lock,std::chrono::milliseconds(m_connect_timeout)))        {
            if(mysql_connect_queue.empty()){
                LOG("连接超时");
                return nullptr;
            }
        }
    }
    shared_ptr<mysql_connect> sp(mysql_connect_queue.front(),
    [&](mysql_connect* pcon){
        unique_lock<mutex> lock(m_mutex);
        pcon->refresh_conn();
        mysql_connect_queue.push(pcon);
    });
    mysql_connect_queue.pop();
    
    m_cond.notify_one();//通知其他线程，有空闲连接了
    return sp;
}

mysql_connect_poll::mysql_connect_poll()
{
    for(int i=0;i<m_init_conn;i++){
        mysql_connect* p=new mysql_connect();
        p->init(m_ip,m_port,m_user,m_password,m_database);
        p->refresh_conn();
        mysql_connect_queue.push(p);
        m_connCnt++;
    }

    thread produce(std::bind(&mysql_connect_poll::produce_connect,this));
    produce.detach();
    thread scanner(std::bind(&mysql_connect_poll::scan_free_connect,this));
    scanner.detach();
}
//运行在独立的线程中，不断生产连接
void mysql_connect_poll::produce_connect(){
    for(;;)
    {
        unique_lock<mutex> lock(m_mutex);
        while(!mysql_connect_queue.empty()){
            m_cond.wait(lock);
        }

        if(m_connCnt<m_max_conn){
            mysql_connect* p=new mysql_connect();
            p->init(m_ip,m_port,m_user,m_password,m_database);
            p->refresh_conn();
            mysql_connect_queue.push(p);
            m_connCnt++;
    }
    m_cond.notify_all();
}
}

//扫描超过一定时间的连接，进行超时处理
void mysql_connect_poll::scan_free_connect(){
    for(;;){
        unique_lock<mutex> lock(m_mutex);
        while(m_connCnt>m_init_conn)
        {
            mysql_connect* p=mysql_connect_queue.front();
            if(p->get_alive_time()>m_max_idle_time){
                mysql_connect_queue.pop();
                m_connCnt--;
                delete p;
            }
            else{
                break;
            }
        }
    }
}
