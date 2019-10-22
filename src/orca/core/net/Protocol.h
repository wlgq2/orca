#ifndef  CORE_PROCOTOL_H
#define  CORE_PROCOTOL_H


namespace orca
{ 
namespace core
{
class Protocol
{
public:
    enum MessageType
    {
        ReqFrameworkId = 1,
        RespFrameworkId,
        ActorMessage,
        HeartBeatMessage
    };

};
}
}
#endif // ! CORE_PROCOTOL_H

