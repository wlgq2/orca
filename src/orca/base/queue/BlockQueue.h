#ifndef  ORCA_QUEUE_H
#define  ORCA_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

namespace orca
{ 
namespace base
{

template <typename Type>
class BlockQueue
{
public:
    bool push(Type ele);
    Type pop();
    bool empty();
    void clear();
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<Type> queue_;
};


template<typename Type>
inline bool BlockQueue<Type>::push(Type ele)
{
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(ele);
    condition_.notify_all();
    return true;
}

template<typename Type>
inline Type BlockQueue<Type>::pop()
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this]() {return !queue_.empty(); });
    auto rst = queue_.front();
    queue_.pop();
    return rst;
}

template<typename Type>
inline bool BlockQueue<Type>::empty()
{
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
}

template<typename Type>
inline void BlockQueue<Type>::clear()
{
    std::unique_lock<std::mutex> lock(mutex_);
    std::queue<Type> empty;
    queue_.swap(empty);
}
}
}
#endif // ! ORCA_QUEUE_H