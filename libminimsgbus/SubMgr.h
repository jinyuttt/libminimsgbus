#pragma once
#include<chrono>
#include <thread>
#include <iostream>
#include <ctime>
#include <ratio>
#include <map>
#include<list>
#include <queue>
#include <mutex>
#include"MsgLocalNode.h"
#include"blockingconcurrentqueue.h"
#include"Util.h"
#include "TopicBroadcast.h"
#include"PubTable.h"
#include"SubTable.h"
#include "NngDataNative.h"
#include "LocalTopic.h"
#include "msgtopic.h"
#include "MsgStruct.h"
using namespace std;
using namespace msgtransport;
namespace libminimsgbus
{
    
    class SubMgr
    {
    private:  //Ĭ�Ϲ���Ϳ������춼��Ҫ��private
        SubMgr();
        SubMgr(const SubMgr& other);
        ~SubMgr();
        NngDataNative* native = new NngDataNative();
    public:
        static SubMgr* GetInstance() {
            static SubMgr instance;
            return &instance;
        }

        /// <summary>
         /// ������
         /// </summary>
        map<string, list<msgtopic*>> dicSubObj;

        /// <summary>
        /// ���յ����ݣ�����������Ϣ
        /// </summary>
        BlockingConcurrentQueue<TopicStruct> topicStructs;

     
        const int waitTime = 1000*60;//1����
       
        /// <summary>
        /// ������Ϣ
        /// </summary>
        map<string, std::chrono::steady_clock::time_point> dicMsg;

        TopicBroadcast *topicBroadcast;
        void Init();

    

    public:
        /// <summary>
        /// ���ÿշ�����ʼ��ʵ��
        /// </summary>
        void openChanel();

       
        /// <summary>
        /// ��ȡ���ص�ַ
        /// </summary>
        /// <param name="address"></param>
        /// <param name="protol"></param>
        /// <param name="ip"></param>
        /// <param name="port"></param>
        void getRealAddress(string address, string& protol, string& ip, int& port);


        /// <summary>
        /// ��ʼ��������ַ�����
        /// </summary>
        void initPgm();
       
     /// <summary>
     /// ��������
     /// </summary>
     /// <param name="topic"></param>
     /// <param name="address"></param>
     static   void TopicBroadcast_ReceiveTopic(string topic, string address);


        /// <summary>
        /// ��������
        /// </summary>
        void  processSub();


        /// <summary>
        /// ��ʼ���������ݣ�׼������������ݣ����ĵ����ݺͶ�����Ϣ��
        /// </summary>
        void initDataRecive();


        /// <summary>
        /// ��ʱ�����ظ���Key=�ڵ��ʶ+msgid
        /// </summary>
        void removeFilter();


        /// <summary>
        /// ���Ͷ�����Ϣ
        /// </summary>
        /// <param name="topic">����</param>
        /// <param name="sub">����</param>
        void  sendSub(string topic, msgtopic* sub);

        void  sendUnsub(string topic);
    };

}
