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
    private:  //默认构造和拷贝构造都需要是private
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
         /// 订阅者
         /// </summary>
        map<string, list<msgtopic*>> dicSubObj;

        /// <summary>
        /// 接收的数据，包括订阅信息
        /// </summary>
        BlockingConcurrentQueue<TopicStruct> topicStructs;

     
        const int waitTime = 1000*60;//1分钟
       
        /// <summary>
        /// 过滤消息
        /// </summary>
        map<string, std::chrono::steady_clock::time_point> dicMsg;

        TopicBroadcast *topicBroadcast;
        void Init();

    

    public:
        /// <summary>
        /// 调用空方法初始化实例
        /// </summary>
        void openChanel();

       
        /// <summary>
        /// 获取本地地址
        /// </summary>
        /// <param name="address"></param>
        /// <param name="protol"></param>
        /// <param name="ip"></param>
        /// <param name="port"></param>
        void getRealAddress(string address, string& protol, string& ip, int& port);


        /// <summary>
        /// 初始化发布地址与接收
        /// </summary>
        void initPgm();
       
     /// <summary>
     /// 接受数据
     /// </summary>
     /// <param name="topic"></param>
     /// <param name="address"></param>
     static   void TopicBroadcast_ReceiveTopic(string topic, string address);


        /// <summary>
        /// 处理数据
        /// </summary>
        void  processSub();


        /// <summary>
        /// 初始化接收数据，准备网络接收数据（订阅的数据和订阅信息）
        /// </summary>
        void initDataRecive();


        /// <summary>
        /// 定时清理重复的Key=节点标识+msgid
        /// </summary>
        void removeFilter();


        /// <summary>
        /// 发送订阅信息
        /// </summary>
        /// <param name="topic">主题</param>
        /// <param name="sub">数据</param>
        void  sendSub(string topic, msgtopic* sub);

        void  sendUnsub(string topic);
    };

}
