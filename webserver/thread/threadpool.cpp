#include "threadpool.h"
threadpool::threadpool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;

                    {   // 进入临界区
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, 
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    // 执行任务
                    task();
                }
            }
        );
}

    //     template<class F, class... Args>
    //     auto threadpool::enqueue(F&& f,Args&&... args)->
    // std::future<typename std::result_of<F(Args...)>::type>
    //     {
    //         using return_type = typename std::result_of<F(Args...)>::type;
        
    //         // 创建一个 packaged_task
    //         auto task = std::make_shared< std::packaged_task<return_type()> >(
    //             std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    //         );
                
    //         std::future<return_type> res = task->get_future();
    //         { // 进入临界区
    //             std::unique_lock<std::mutex> lock(queue_mutex);
        
    //             // 不允许在停止后添加任务
    //             if(stop)
    //                 throw std::runtime_error("enqueue on stopped ThreadPool");
        
    //             tasks.emplace([task](){ (*task)(); });
    //         }
    //         condition.notify_one();
    //         return res;
    //     }
    
        threadpool::~threadpool()
        {
            { // 进入临界区
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for(std::thread &worker: workers)
                worker.join();
        }
        