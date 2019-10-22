#include  "ThreadPool.h"

using namespace std;
using namespace orca::base;


ThreadPool::ThreadPool()
{
}

ThreadPool::~ThreadPool()
{
    threads_.clear();
}


void ThreadPool::run()
{
    while (true)
    {
        processor_();
    }
}


void ThreadPool::start(unsigned int cnt)
{
    for (unsigned int i = 0; i < cnt; i++)
    {
        ThreadPtr ptr = std::make_shared<orca::base::Thread>(std::bind(&ThreadPool::run, this));
        threads_.push_back(ptr);
        ptr->start();
    }
}

void orca::base::ThreadPool::join()
{
    for (auto thread : threads_)
    {
        thread->join();
    }
}

void ThreadPool::registerPorcess(ProcessorType process)
{
    processor_ = process;
}

