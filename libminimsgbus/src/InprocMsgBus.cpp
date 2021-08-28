#include "pch.h"
#include "InprocMsgBus.h"
namespace libminimsgbus
{
    void InprocMsgBus::revmsgtopic(string topic, char* data, int len)
    {
        if (revmsg != nullptr)
        {
            revmsg(topic, data, len);
        }
    }
    uint64_t InprocMsgBus::publish(string topic, char bytes[], int len)
    {

        if (topic.empty())
        {
            topic = defaultTopic;
        }
        if (strlen(bytes) == 0)
        {
            return 0;
        }
        int size = sizeof(bytes) / sizeof(bytes[0]);
        myevent.Notify(topic, bytes, size);

        return 0;
    }
    void InprocMsgBus::subscribe(string topic)
    {
        if (topic.empty())
        {
            topic = defaultTopic;
        }
        //std::function зЂВс
        std::function<void(string, char*, int)> f = std::bind(&InprocMsgBus::revmsgtopic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        myevent.Connect(f);
    }
  
	
}
