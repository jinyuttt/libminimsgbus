#pragma once
#include <string>
#include "PubMgr.h"
#include "MsgDef.h"
using namespace std;

namespace libminimsgbus
{

    /// <summary>
    /// 
    /// </summary>
    class subscriber;

    /// <summary>
    /// 订阅数据
    /// </summary>
    class msgtopic
    {
      
        subscriber *suber;
    public:
      
        ReceiveTopic rectopic=nullptr;
        uint64_t subid = 0;
      
        FBC_API  msgtopic();

        FBC_API  ~msgtopic();

        /// <summary>
        /// 订阅主题
        /// </summary>
        FBC_API  void subscribe(string topic);
    

        /// <summary>
        /// 发布
        /// </summary>
        /// <param name="topic">主题</param>
        /// <param name="msg">数据</param>
        FBC_API   int64_t publish(string topic, char msg[],int len);
      

        /// <summary>
        /// 内部数据回传
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="msg"></param>
        void addtopic(string topic, char msg[],int len);
    
    };

};

