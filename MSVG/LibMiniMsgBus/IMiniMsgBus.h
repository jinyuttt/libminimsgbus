#pragma once
#include <string>
using namespace std;
namespace libminimsgbus
{
    typedef void (*ReceiveTopic)(std::string, char*, int);
    class IMiniMsgBus
    {

    protected:
        const string defaultTopic = "defaultTopicBus";
        long objPoint = 0;
        IMiniMsgBus();
    public:

        /// <summary>
        /// ¶©ÔÄ»Øµ÷
        /// </summary>
        ReceiveTopic  revmsg=nullptr;
        virtual  uint64_t publish(string topic, char bytes[],int len) = 0;

        virtual    void subscribe(string topic) = 0;
    };
}

