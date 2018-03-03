#ifndef   ORCA_DEFINE_H
#define   ORCA_DEFINE_H

#define      USED_LIB_UV       0


#define      USED_SPD_LOG      0

#ifdef _MSC_VER
//#define 
#endif // _MSC_VER


#define REGISTER_MESSAGE_TYPE(type)                                       \
namespace orca                                                            \
{                                                                         \
using MESSAGE_TYPE = type;                                                \
using MessagePack = orca::core::MessagePack<>;                            \
using Actor = orca::core::Actor<type>;                                    \
using Framework = orca::core::Framework<type>;                            \
using Address = orca::core::Address;                                      \
}

#endif // !  ORCA_DEFINE_H

