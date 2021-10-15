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

         /// <summary>
         /// ��������
         /// </summary>
         /// <param name="url">��ַ</param>
         /// <param name="topic">����</param>
         /// <param name="bytes">����</param>
         /// <param name="len">���ݳ���</param>
         void publish(string url,string topic, char* bytes, int len);

         /// <summary>
         /// ����
         /// </summary>
         /// <param name="url">��ַ</param>
         /// <param name="topic">����</param>
         void subscribe(string url, string topic);

		 /// <summary>
		 /// �ڲ���Ϣ����
		 /// </summary>
		 void recmsg();

		 /// <summary>
		 /// ���涩�Ķ���
		 /// </summary>
		 /// <param name="bus"></param>
		 void add(MqMsgBus* bus);

         /// <summary>
         /// �Ƴ�
         /// </summary>
         /// <param name="bus"></param>
         void remove(MqMsgBus* bus);
	};
}

