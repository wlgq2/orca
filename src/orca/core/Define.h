#ifndef   ORCA_DEFINE_H
#define   ORCA_DEFINE_H

#include   <mutex>

#define      USED_LIB_UV       0


#define      USED_SPD_LOG      0

#ifdef _MSC_VER
//#define 
#endif // _MSC_VER

//Ê¹ÓÃ×ÔÐýËø
#define         USE_SPIN_LOCK              1
#if  USE_SPIN_LOCK

namespace orca
{
namespace base
{ 
class SpinLock;
using CommonLockType = SpinLock;
}
}

#else
using CommodLockType = std::mutex;
#endif




#define REGISTER_MESSAGE_TYPE(type)                  \
namespace orca                                       \
{                                                    \
using MessagePack = orca::core::MessagePack<type>;   \
using Actor = orca::core::Actor<type>;               \
using Framework = orca::core::Framework<type>;       \
using Address = orca::core::Address;                 \
using FrameworkConfig = orca::core::FrameworkConfig;   \
using EndPointAddress = orca::core::EndPointAddress;  \
}

#endif // !  ORCA_DEFINE_H

