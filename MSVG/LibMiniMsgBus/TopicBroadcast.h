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
        /// 初始化本地地址
        /// </summary>
      void initAddress();
       
        /// <summary>
        /// 获取地址
        /// </summary>
      void getLocalAddress();
      


        /// <summary>
        /// 处理接收的数据
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
      void getTopicData(string obj, string& topic, string& address);
       

        /// <summary>
        /// 订阅数据
        /// </summary>
      void topicSub();
      

      void ipc_ReceiveTopic(string arg1, char* arg2,int len);
       

      void pgm_ReceiveTopic(string arg1, char* arg2,int len);
      

        /// <summary>
        ///  通知主题本节点发布地址
        /// </summary>
        /// <param name="topic">发布的主题</param>
        /// <returns>本地发布地址</returns>
      list<string> pgmPub(string topic);
       



      void broadcast(bool ispgm);
       
    };
};

