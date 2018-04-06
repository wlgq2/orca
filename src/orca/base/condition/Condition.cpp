#include "Condition.h"

void orca::base::Condition::notifyAll()
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.notify_all();
}

void orca::base::Condition::notify()
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.notify_one();
}

void orca::base::Condition::wait(ConditionCallback callback)
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, callback);
}
