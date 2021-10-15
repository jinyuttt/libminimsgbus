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
    private:  //Ĭ�Ϲ���Ϳ������춼��Ҫ��private
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
         void publish(string url,string topic, char* bytes, int len);
         void subscribe(string url, string topic);
		 void recmsg();
		 void add(MqMsgBus* bus);
	};
}

