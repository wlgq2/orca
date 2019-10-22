#ifndef  ORCA_CONDITION_H
#define  ORCA_CONDITION_H

#include <mutex>
#include <condition_variable>
#include <functional>

namespace orca
{ 
namespace base
{

class Condition
{
public:
    using ConditionCallback = std::function<bool()>;

    void notifyAll();
    void notify();
    void wait(ConditionCallback callback);
private:
    std::mutex mutex_;
    std::condition_variable condition_;
};
}
}
#endif