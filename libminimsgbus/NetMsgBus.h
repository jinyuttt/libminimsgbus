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
      
        // Í¨¹ý IMiniMsgBus ¼Ì³Ð
        virtual uint64_t publish(string topic, char bytes[], int len) override;

        virtual void subscribe(string topic) override;

    };
}

