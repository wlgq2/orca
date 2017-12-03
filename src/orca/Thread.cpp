#include <orca/Thread.h>


using namespace std;
using namespace orca;

Thread::Thread()
    :Thread(nullptr)
{

}

Thread::Thread(std::function<void ()> callback)
    :started_(false),
    run_(callback),
    thread_(std::bind(&Thread::entryPoint,this))
{

}

Thread::~Thread()
{

}

void Thread::start()
{
    unique_lock<mutex> lock(mutex_);
    started_ = true;
    condition_.notify_one();
}

void Thread::join()
{
    thread_.join();
}
void Thread::detach()
{
    thread_.detach();
}

bool Thread::isRunInThisThread()
{
    return (CurrentThreadId() == id_);
}

std::thread::id Thread::thisThreadId()
{
    return id_;
}

void Thread::registerProcess(std::function<void()> func)
{
    run_ = func;
}

void Thread::entryPoint()
{

    id_ = CurrentThreadId();
    {
        unique_lock<mutex> lock(mutex_);
        condition_.wait(lock,[this]{return started_;});
    }
    if(run_)
        run_();
}


bool Thread::isStarted()
{
    return started_;
}

void Thread::SleepMSeconds(unsigned long ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


std::thread::id Thread::CurrentThreadId()
{
    return std::this_thread::get_id();
}

void Thread::Yield()
{
    std::this_thread::yield();
}