#ifndef  ORCA_ADDRESS_H
#define  ORCA_ADDRESS_H

#include  <string>

namespace orca
{
namespace core
{ 

struct Address
{
    uint32_t framework;
    uint32_t page;
    uint32_t index;
};
}
}
#endif // ! ORCA_ADDRESS_H

