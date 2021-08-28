#pragma once
#include "IPtPNet.h"
#include "BlockingConcurrentQueue.h"
#include <NngDataNative.h>
using namespace moodycamel;
using namespace msgtransport;
namespace libminimsgbus
{
    struct  PtpBuffer
    {
        int len;
        char* data;
    };

    class PtPNet :
        public IPtPNet
    {
    private:
     
         string sendAddress;

        bool isStart = false;

        /// <summary>
        /// ���յ�����
        /// </summary>
          BlockingConcurrentQueue<PtpBuffer> topicStructs ;
  
          void getRealAddress(string address, string &protol, string &ip, int &port);
          void processData();
          void initDataRecive(string localIP, int port);
    public:
      
        // ͨ�� IPtPNet �̳�
        virtual void send(char data[]) override;
        virtual void send(string addr, char data[]) override;
        virtual void start() override;
    };

   
}
