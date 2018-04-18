#ifndef    SPIN_LOCK_H
#define    SPIN_LOCK_H

#include <atomic>
namespace orca
{ 
namespace base
{ 

class SpinLock
{
public:
    enum
    {
        Unlocked,
        Locked
    };

    SpinLock();
    void lock();
    void unlock();
private:
    std::atomic<int> locked_;
};
}
}
#endif // ! SPIN_LOCK_H

