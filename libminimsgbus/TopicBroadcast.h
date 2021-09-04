#pragma once
#include <map>
#include <string>
#include<list>
#include "Util.h"
#include "MsgLocalNode.h"
#include <IpcNative.h>
#include <ZmqPgm.h>
#include "PubTable.h"
#include<set>
using namespace std;
using namespace msgtransport;
namespace libminimsgbus
{
    typedef  void (*IpcReceiveTopic)(std::string, std::string);
    class TopicBroadcast
    {
       IpcNative *ipc;
       ZmqPgm *pgm;
      static std::mutex mtx;
      public:
        static list<string> lstNodeAddress;
        static  list<string> lstBindIP;
        IpcReceiveTopic revTopic=nullptr;
        TopicBroadcast();
        ~TopicBroadcast();

        /// <summary>
        /// ��ʼ�����ص�ַ
        /// </summary>
      void initAddress();
       
        /// <summary>
        /// ��ȡ��ַ
        /// </summary>
      void getLocalAddress();
      


        /// <summary>
        /// ������յ�����
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
      void getTopicData(string obj, string& topic, string& address);
       

        /// <summary>
        /// ��������
        /// </summary>
      void topicSub();
      

      void ipc_ReceiveTopic(string arg1, char* arg2,int len);
       

      void pgm_ReceiveTopic(string arg1, char* arg2,int len);
      

        /// <summary>
        ///  ֪ͨ���Ȿ�ڵ㷢����ַ
        /// </summary>
        /// <param name="topic">����������</param>
        /// <returns>���ط�����ַ</returns>
      list<string> pgmPub(string topic);
       



      void broadcast(bool ispgm);
       
    };
};

