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
        // 通过 IMiniMsgBus 继承
        void revmsgtopic(string topic, char* data, int len);
    public:
     

        // 通过 IMiniMsgBus 继承
        virtual uint64_t publish(string topic, char bytes[], int len) override;

        virtual void subscribe(string topic) override;

    };
}

