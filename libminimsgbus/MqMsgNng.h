#pragma once
#include <string>
#include <list>
#include <thread>
#include "MqMsgBus.h"
using namespace std;
using namespace msgtransport;
namespace libminimsgbus
{
    class MqMsgBus;
	class MqMsgNng
	{
    private:  //默认构造和拷贝构造都需要是private
        MqMsgNng();
        MqMsgNng(const MqMsgNng& other);
        ~MqMsgNng();
        NngDataNative* nng =nullptr;

       
    public:
        static MqMsgNng* GetInstance() {
            static MqMsgNng instance;
            return &instance;
        }
		 list<MqMsgBus*> lstmq;

         /// <summary>
         /// 发布数据
         /// </summary>
         /// <param name="url">地址</param>
         /// <param name="topic">主题</param>
         /// <param name="bytes">数据</param>
         /// <param name="len">数据长度</param>
         void publish(string url,string topic, char* bytes, int len);

         /// <summary>
         /// 订阅
         /// </summary>
         /// <param name="url">地址</param>
         /// <param name="topic">主题</param>
         void subscribe(string url, string topic);

		 /// <summary>
		 /// 内部消息接受
		 /// </summary>
		 void recmsg();

		 /// <summary>
		 /// 保存订阅对象
		 /// </summary>
		 /// <param name="bus"></param>
		 void add(MqMsgBus* bus);

         /// <summary>
         /// 移除
         /// </summary>
         /// <param name="bus"></param>
         void remove(MqMsgBus* bus);
	};
}

