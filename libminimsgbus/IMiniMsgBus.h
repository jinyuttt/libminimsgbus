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
        /// ���Ļص�
        /// </summary>
        ReceiveTopic  revmsg=nullptr;

        /// <summary>
        /// ���ķ���
        /// </summary>
        std::function<void(std::string, char*, int)> callback;

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="topic">����</param>
        /// <param name="bytes">����</param>
        /// <param name="len">���ݳ���</param>
        /// <returns></returns>
        virtual  uint64_t publish(string topic, char bytes[],int len) = 0;

        /// <summary>
        /// ����
        /// </summary>
        /// <param name="topic">����</param>
        virtual    void subscribe(string topic) = 0;
    };
}

