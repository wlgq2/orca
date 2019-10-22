#ifndef   ORCA_ASSERT_H
#define   ORCA_ASSERT_H

#include  <iostream>
#include  <cassert>

#define ORCA_FAIL()                               orca::Assert::Fail(__FILE__, __LINE__)

#define ORCA_FAIL_MSG(msg)                        orca::Assert::Fail(__FILE__, __LINE__, msg)

#define ORCA_ASSERT(condition)                    orca::Assert::IsFail(condition,__FILE__, __LINE__)

#define ORCA_ASSERT_MSG(condition, msg)           orca::Assert::IsFail(condition,__FILE__, __LINE__,msg)


namespace orca
{

class Assert
{
public:
    static void IsFail(bool nomal, const char* const file, const unsigned int line, const std::string message = "")
    {
        if (!nomal)
        {
            Fail(file,line,message);
        }
    }

    static void Fail(const char* const file, const unsigned int line, const std::string message  = "")
    {
        std::cerr<<"fail in :"<<file<<" at "<<line<<" lines.";
        if ("" != message)
        {
            std::cerr<<"\n:" << message;
        }

        std::cerr << "\n";
        assert(false);
    }
};
}
#endif
