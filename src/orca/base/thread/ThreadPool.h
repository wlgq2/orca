#ifndef    ORCA_THREAD_POOL_H
#define    ORCA_THREAD_POOL_H


#include   <vector>
#include   <iostream>
#include   <functional>

#include   "Thread.h"
namespace orca
{

namespace base
{ 

class ThreadPool
{
public:
    using ProcessorType = std::function<void()>;
    ThreadPool();
    virtual ~ThreadPool();
    using  ThreadPtr = std::shared_ptr<orca::base::Thread>;

    void registerPorcess(ProcessorType process);

    void start(unsigned int cnt);
    void join();
private:
    ProcessorType processor_;
    std::vector<ThreadPtr> threads_;


    void run();
};

}
}
#endif // !   ORCA_THREAD_POOL
