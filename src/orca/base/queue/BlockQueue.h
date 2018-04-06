#ifndef  ORCA_QUEUE_H
#define  ORCA_QUEUE_H

#include  <iostream>
#include  <queue>
#include  <atomic>

#include  "../condition/Condition.h"

namespace orca
{ 
namespace base
{

template <typename Type,typename LockType>
class BlockQueue
{
public:
    BlockQueue();
    void push(Type& ele);
    
    template<class... _Types >
    void push(_Types&&... _Args);
    
    void pop(Type& ele);
    bool empty();
    void clear();
private:
    LockType lock_;
    Condition condition_;
    std::atomic<bool> waited_;
    std::queue<Type> queue_;
};


template<typename Type, typename LockType>
inline BlockQueue<Type, LockType>::BlockQueue()
    :waited_(false)
{
}

template<typename Type, typename LockType>
inline void BlockQueue<Type, LockType>::push(Type& ele)
{
    {
        std::unique_lock<LockType> lock(lock_);
        queue_.push(ele);
    }
    if (waited_)
    {
        condition_.notifyAll();
    }
}

template<typename Type, typename LockType>
inline void BlockQueue<Type, LockType>::pop(Type& ele)
{
    {
        std::unique_lock<LockType> lock(lock_);
        if (!queue_.empty())
        {
            ele = queue_.front();
            queue_.pop();
            return;
        }
    }
    //只有在需要阻塞时才使用互斥锁,否则只适用自旋锁。
    condition_.wait([this]() 
    {
        waited_ = empty();
        return !waited_;
    });
    std::unique_lock<LockType> lock(lock_);
    ele = queue_.front();
    queue_.pop();
}

template<typename Type, typename LockType>
inline bool BlockQueue<Type, LockType>::empty()
{
    std::unique_lock<LockType> lock(lock_);
    return queue_.empty();
}

template<typename Type, typename LockType>
inline void BlockQueue<Type, LockType>::clear()
{
    std::unique_lock<LockType> lock(lock_);
    std::queue<Type> empty;
    queue_.swap(empty);
}


template<typename Type, typename LockType>
template<class ..._Types>
inline void BlockQueue<Type, LockType>::push(_Types && ..._Args)
{
    {
        std::unique_lock<LockType> lock(lock_);
        queue_.push({ _STD forward<_Types>(_Args)... });
    }
    if (waited_)
    {
        condition_.notifyAll();
    }

    
}

}
}
#endif // ! ORCA_QUEUE_H