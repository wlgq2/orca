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
    static const char* Enter(T& object)
    {
        return object.enter();
    }
};

template<>
struct MessageTraits<std::string>
{
    static const char* Enter(std::string& object)
    {
        return (object.c_str());
    }
};
}
}
#endif
