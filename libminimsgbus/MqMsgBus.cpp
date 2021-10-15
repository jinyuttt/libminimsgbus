#include "MqMsgBus.h"
namespace libminimsgbus
{
    MqMsgBus::~MqMsgBus()
    {
        MqMsgNng::GetInstance()->remove(this);
    }
    void MqMsgBus::revmsgtopic(string topic, char* data, int len)
    {
        auto find = mapTopic.find(topic);
        if (find == mapTopic.end())
        {
            return;
        }
        if (callback!=nullptr)
        {
            callback(topic, data, len);
        }
        if (revmsg != nullptr)
        {
            revmsg(topic, data, len);
        }
    }
   
    uint64_t MqMsgBus::publish(string topic, char* bytes, int len)
    {
        
        MqMsgNng::GetInstance()->publish(url, topic, bytes, len);
        return 0;
    }
    void MqMsgBus::subscribe(string topic)
    {
        MqMsgNng::GetInstance()->subscribe(url, topic);
        mapTopic[topic] = "";
        if (isInit)
        {
            isInit = false;
            MqMsgNng::GetInstance()->add(this);
           
           //  std::bind(&MqMsgBus::revmsgtopic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        }
    }
    void MqMsgBus::unsubscribe(string topic)
    {
        mapTopic.erase(topic);
        if (mapTopic.empty())
        {
            MqMsgNng::GetInstance()->remove(this);
        }
    }
}