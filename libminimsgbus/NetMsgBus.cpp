
#include "NetMsgBus.h"
#include"ObjSubMgr.h"
namespace libminimsgbus
{
    NetMsgBus::~NetMsgBus()
    {
       
        msgTopic.rectopic = nullptr;
       auto lst= ObjSubMgr::erase(this);
       for (auto topic : lst)
       {
           msgTopic.unsubscribe(topic);
       }
    }
    uint64_t NetMsgBus::publish(string topic, char* bytes, int len)
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
    void NetMsgBus::unsubscribe(string topic)
    {
        ObjSubMgr::remove(topic, this);
        auto isf = ObjSubMgr::empty(topic);
        if (isf)
        {
            msgTopic.unsubscribe(topic);
        }

    }
}