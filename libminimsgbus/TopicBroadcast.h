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

        /// <summary>
        /// �ڵ�����IP
        /// </summary>
        static list<string> lstNodeAddress;

        /// <summary>
        /// �����󶨵�ַ
        /// </summary>
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
      
      /// <summary>
      /// ipc����
      /// </summary>
      /// <param name="arg1"></param>
      /// <param name="arg2"></param>
      /// <param name="len"></param>
      void ipc_ReceiveTopic(string arg1, char* arg2,int len);
       
      /// <summary>
      /// pgm����
      /// </summary>
      /// <param name="arg1"></param>
      /// <param name="arg2"></param>
      /// <param name="len"></param>
      void pgm_ReceiveTopic(string arg1, char* arg2,int len);
      

        /// <summary>
        ///  ֪ͨ���Ȿ�ڵ㷢����ַ
        /// </summary>
        /// <param name="topic">����������</param>
        /// <returns>���ط�����ַ</returns>
      list<string> pgmPub(string topic);
       


      /// <summary>
      /// �㲥���е�ַ
      /// </summary>
      /// <param name="ispgm"></param>
      void broadcast(bool ispgm);
       
    };
};

