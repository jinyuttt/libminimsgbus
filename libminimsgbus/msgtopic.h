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
    /// ��������
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
        /// ��������
        /// </summary>
        FBC_API  void subscribe(string topic);
    

        /// <summary>
        /// ����
        /// </summary>
        /// <param name="topic">����</param>
        /// <param name="msg">����</param>
        FBC_API   int64_t publish(string topic, char msg[],int len);
      

        /// <summary>
        /// �ڲ����ݻش�
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="msg"></param>
        void addtopic(string topic, char msg[],int len);
    
    };

};

