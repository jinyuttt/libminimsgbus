#pragma once
#include "IMiniMsgBus.h"
#include "NonCopyable.h"
using namespace std;
namespace libminimsgbus
{
    class InprocMsgBus :
        public IMiniMsgBus
    {
    private:
        map<string, string> dic;
        Events<std::function<void(string, char*,int)> > myevent;
        // ͨ�� IMiniMsgBus �̳�
        void revmsgtopic(string topic, char* data, int len);
    public:
     

        // ͨ�� IMiniMsgBus �̳�
        virtual uint64_t publish(string topic, char bytes[], int len) override;

        virtual void subscribe(string topic) override;

    };
}

