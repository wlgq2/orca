#ifndef    ORCA_MESSAGE_TRAITS_H
#define    ORCA_MESSAGE_TRAITS_H

#include <string>

namespace orca
{
namespace core
{ 

template<typename T>
struct MessageTraits
{
    static const void* Enter(T& object)
    {
        return object.enter();
    }
};

template<>
struct MessageTraits<std::string>
{
    static const void* Enter(std::string& object)
    {
        return static_cast<const void*>(object.c_str());
    }
};
}
}
#endif
