#pragma once
#include<vector>
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>//条件变量
#include<functional>//函数对象
#include<future>//异步任务
class threadpool{
public:
    threadpool(size_t threads);
    ~threadpool();

    //添加任务
    //F:函数类型
    //Args:函数参数类型
    //返回值:任务函数返回值类型
    template<class F,class... Args>//函数对象
    auto enqueue(F&& f,Args&&... args)->
    std::future<typename std::result_of<F(Args...)>::type>;

private:
    //工作线程
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    //同步
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

template<class F, class... Args>
auto threadpool::enqueue(F&& f,Args&&... args)->
std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    // 创建一个 packaged_task
    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
        
    std::future<return_type> res = task->get_future();
    { // 进入临界区
        std::unique_lock<std::mutex> lock(queue_mutex);

        // 不允许在停止后添加任务
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}