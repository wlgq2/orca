#ifndef    ORCA_MESSAGE_TRAITS
#define    ORCA_MESSAGE_TRAITS

#include <string>

namespace orca
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
#endif
