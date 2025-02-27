
#include "mysql_connect.h"
#include "public.h"

mysql_connect::mysql_connect(){
    m_conn=mysql_init(NULL);
}

mysql_connect::~mysql_connect(){
    if(m_conn!=nullptr)
    {
        mysql_close(m_conn);
    }
}

bool mysql_connect::init(string ip,unsigned short port,string user,string password,string database){
    
    MYSQL* p=mysql_real_connect(m_conn,ip.c_str(),user.c_str(),password.c_str(),database.c_str(),port,NULL,0);
    if(p==nullptr){
        return false;
    }
    return true;
}

bool mysql_connect::update(string sql){
    int ret=mysql_query(m_conn,sql.c_str());
    if(ret!=0){
        return false;
    }
    return true;
}

MYSQL_RES* mysql_connect::query(string sql){
    int ret=mysql_query(m_conn,sql.c_str());
    if(ret!=0){
        return nullptr;
    }
    return mysql_use_result(m_conn);
}
