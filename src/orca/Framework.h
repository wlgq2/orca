#ifndef    ORCA_FRAMEWORK_H
#define    ORCA_FRAMEWORK_H

#include   <orca/ThreadPool.h>
#include   <orca/MessagePack.h>
#include   <orca/Actor.h>

namespace orca
{
namespace core
{ 

class Framework
{
public:
    Framework()
    {
        //pool_.registerPorcess(std::bind(&Framework::process,this));
        //pool_.start(2);
    }

    void process()
    {
        while (true)
        {
            Thread::SleepMSeconds(10000);
        }
    }
    


private:
    //ThreadPool<std::queue<int>,std::function<void()>> pool_;


   
};

}
}
#endif // ! ORCA_FRAMEWORK_H
