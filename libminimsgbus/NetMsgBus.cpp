
#include "NetMsgBus.h"
#include"ObjSubMgr.h"
namespace libminimsgbus
{
  
    uint64_t NetMsgBus::publish(string topic, char bytes[], int len)
    {
        if (topic.empty())
        {
            topic = defaultTopic;
        }
        return  msgTopic.publish(topic, bytes,len);
    }

    void NetMsgBus::subscribe(string topic)
    {
        if (topic.empty())
        {
            topic = defaultTopic;
        }
        if (isInit)
        {
            msgTopic.rectopic = &ObjSubMgr::receiveTopic;
            objPoint = ObjSubMgr::getSubscriber();
            isInit = false;
         
        }
        ObjSubMgr::holdTopic(topic, this);
        msgTopic.subscribe(topic);
    }
}