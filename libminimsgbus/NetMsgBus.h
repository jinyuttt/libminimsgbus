#pragma once
#include "IMiniMsgBus.h"
#include "msgtopic.h"
namespace libminimsgbus
{
    class NetMsgBus :
        public IMiniMsgBus
    {
        msgtopic msgTopic;
        bool isInit = true;
      
        // 通过 IMiniMsgBus 继承
        virtual uint64_t publish(string topic, char bytes[], int len) override;

        virtual void subscribe(string topic) override;


        // 通过 IMiniMsgBus 继承
        virtual void unsubscribe(string topic) override;

    };
}

