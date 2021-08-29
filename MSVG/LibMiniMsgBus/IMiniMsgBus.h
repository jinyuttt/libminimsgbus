#pragma once
#include <string>
#include <functional>
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
        /// 订阅回调
        /// </summary>
        ReceiveTopic  revmsg=nullptr;

        /// <summary>
        /// 订阅返回
        /// </summary>
        std::function<void(std::string, char*, int)> callback;

        /// <summary>
        /// 发布数据
        /// </summary>
        /// <param name="topic">主题</param>
        /// <param name="bytes">数据</param>
        /// <param name="len">数据长度</param>
        /// <returns></returns>
        virtual  uint64_t publish(string topic, char bytes[],int len) = 0;

        /// <summary>
        /// 订阅
        /// </summary>
        /// <param name="topic">主题</param>
        virtual    void subscribe(string topic) = 0;
    };
}

