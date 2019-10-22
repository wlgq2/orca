#ifndef ORCA_THREAD_H
#define ORCA_THREAD_H

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace  orca
{ 

namespace base
{ 
class Thread
{
public:
    Thread();
    Thread(std::function<void ()> callback);

    virtual ~Thread();

    void start();

    virtual void stop() {}

    bool isStarted();

    void join();
    void detach();

    bool isRunInThisThread();

    std::thread::id thisThreadId();

    void registerProcess(std::function<void()> func);

    static void YieldCurrentThread();
    static std::thread::id CurrentThreadId();
    static void SleepMSeconds(unsigned long ms);

protected:
    bool started_;
    std::mutex mutex_;
    std::condition_variable condition_;
    
private:
    std::function<void()> run_;
    std::thread::id id_;
    std::thread thread_;

    void entryPoint();
};
}
}
#endif
