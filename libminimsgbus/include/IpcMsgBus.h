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
       

      


        // ͨ�� IMiniMsgBus �̳�
        virtual uint64_t publish(string topic, char bytes[], int len) override;

        virtual void subscribe(string topic) override;

    };
}

