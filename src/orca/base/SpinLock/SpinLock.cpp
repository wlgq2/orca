#include "SpinLock.h"

#include <thread>
using namespace orca::base;

SpinLock::SpinLock()
    :locked_(Unlocked)
{
}

void SpinLock::lock()
{
    while (true)
    {
        int old(Unlocked);
        if (locked_.compare_exchange_strong(old, Locked))
        {
            return;
        }
        std::this_thread::yield();
    }
}

void SpinLock::unlock()
{
    locked_.store(Unlocked);
}
