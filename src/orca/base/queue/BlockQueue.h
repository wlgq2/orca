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
    void push(Type& ele);
    
    template<class... _Types >
    void push(_Types&&... _Args);
    
    void pop(Type& ele);
    bool empty();
    void clear();
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<Type> queue_;
};


template<typename Type>
inline void BlockQueue<Type>::push(Type& ele)
{
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(ele);
    condition_.notify_all();
}

template<typename Type>
inline void BlockQueue<Type>::pop(Type& ele)
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this]() {return !queue_.empty(); });
    ele = queue_.front();
    queue_.pop();
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


template<typename Type>
template<class ..._Types>
inline void BlockQueue<Type>::push(_Types && ..._Args)
{
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push({ _STD forward<_Types>(_Args)... });
    condition_.notify_all();
}

}
}
#endif // ! ORCA_QUEUE_H