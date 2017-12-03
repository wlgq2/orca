#ifndef  ORCA_ADDRESS_H
#define  ORCA_ADDRESS_H

#include  <string>

namespace orca
{
namespace core
{ 

struct Index
{
    unsigned int framework;
    unsigned int actor;
};

class Address
{
public:
    Address()
    {
    }

    ~Address(){}

private:
    std::string name_;
    struct Index index_;

};
}
}
#endif // ! ORCA_ADDRESS_H

