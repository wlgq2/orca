#ifndef    ORCA_THREAD_POOL_H
#define    ORCA_THREAD_POOL_H

#include   "Thread.h"
#include   <vector>
#include   <iostream>


#include <map>
namespace orca
{

namespace base
{ 
template<typename QueueType,typename ProcessorType>
class ThreadPool
{
public:
    ThreadPool();
    virtual ~ThreadPool();
    using  ThreadPtr = std::shared_ptr<orca::Thread>;

    void start(unsigned int cnt);

    void registerPorcess(ProcessorType process)
    {
        processor_ = process;
    }

    int getIII(int i)
    {
        return test[i];
    }

    int invail(int num)
    {
        for (auto i = 0; i < 5000; i++)
        {
            num += i;
            num *= i;
        }
        return num;
    }
private:
    QueueType* queue_;
    ProcessorType processor_;
    std::vector<ThreadPtr> threads_;


    void run();

    /***/
    std::map<int, int> test;
};



template<typename QueueType, typename ProcessorType>
ThreadPool<QueueType, ProcessorType>::ThreadPool()
{
    for (auto i = 0; i < 10000; i++)
    {
        test[i] = i;
    }
}

template<typename QueueType, typename ProcessorType>
ThreadPool<QueueType, ProcessorType>::~ThreadPool()
{
    threads_.clear();
}

template<typename QueueType, typename ProcessorType>
void ThreadPool<QueueType, ProcessorType>::run()
{
    while (true)
    {
        processor_();
    }
}

template<typename QueueType, typename ProcessorType>
void ThreadPool<QueueType, ProcessorType>::start(unsigned int cnt)
{
    for(auto i = 0; i < cnt; i++)
    {
        ThreadPtr ptr = std::make_shared<orca::Thread>(std::bind(&ThreadPool::run,this));
        threads_.push_back(ptr);
        ptr->start();
    }
}

}
}
#endif // !   ORCA_THREAD_POOL

