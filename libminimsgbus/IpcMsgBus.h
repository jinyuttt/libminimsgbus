#pragma once
#include "IMiniMsgBus.h"
#include <IpcNative.h>
#include<map>
using namespace msgtransport;
namespace libminimsgbus
{
    class IpcMsgBus :
        public IMiniMsgBus
    {
    private:
        IpcNative topicIpc;
        volatile bool isInit = true;
    public:

        // 通过 IMiniMsgBus 继承
        virtual uint64_t publish(string topic, char bytes[], int len) override;

        virtual void subscribe(string topic) override;


        // 通过 IMiniMsgBus 继承
        virtual void unsubscribe(string topic) override;

    };
}

