#ifndef  ORCA_ADDRESS_H
#define  ORCA_ADDRESS_H

#include  <string>

namespace orca
{
namespace core
{ 

struct Address
{
    unsigned int framework;
    unsigned int page;
    unsigned int index;
};
}
}
#endif // ! ORCA_ADDRESS_H

