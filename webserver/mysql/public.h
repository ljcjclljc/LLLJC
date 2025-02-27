#include<iostream>
#include"mysql_connect.h"
#include<string>
#include<queue>
#include<map>
#include<set>
#include<list>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<atomic>
#include<functional>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/epoll.h>

using std::cout;
using std::endl;

#define LOG(str) cout<<str<<endl;