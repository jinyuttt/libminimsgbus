#include "MqMsgBus.h"
namespace libminimsgbus
{
    void MqMsgBus::revmsgtopic(string topic, char* data, int len)
    {
        if (callback!=nullptr)
        {
            callback(topic, data, len);
        }
        if (revmsg != nullptr)
        {
            revmsg(topic, data, len);
        }
    }
    uint64_t MqMsgBus::publish(string topic, char bytes[], int len)
    {
        mq.address = url;
        mq.publish(topic, bytes, len);
        return 0;
    }
    void MqMsgBus::subscribe(string topic)
    {
        mq.address = url;
        mq.subscribe(topic);
        if (isInit)
        {
            isInit = false;
            mq.callback= std::bind(&MqMsgBus::revmsgtopic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        }
    }
    void MqMsgBus::unsubscribe(string topic)
    {

    }
}