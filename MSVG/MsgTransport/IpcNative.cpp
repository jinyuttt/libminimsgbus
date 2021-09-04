#include "pch.h"
#include "IpcNative.h"
#include <thread>
#include<array>
#include <sstream> 
#include<strstream>
#include <mutex>
#include <Tools.h>
namespace msgtransport
{
	void IpcNative::ReceiveData()
	{
        std::lock_guard<std::mutex> l(sigRec);
        if (isStart)
        {
            return;
        }
        thread recTopic(&IpcNative::ReceiveThread,this);
        recTopic.detach();
	}
    void IpcNative::ReceiveThread()
    {
        if (isStart)
        {
            return;
        }
        int bufLen = 0;
        isStart = true;
       // std::string buf, id = id_ + std::to_string(calc_unique_id());
       // std::regex  reg{ "(c\\d+)> (.*)" };
        while (true)
        {
            ipc::buff_t buf = receiver_.recv();
           
            
            if (buf.empty()) break; // quit
            auto dd = buf.get<char const*>();
            std::string dat{ buf.get<char const*>(), buf.size() };
          
            std::istringstream isoo(dat);
            char* buffer = new char[4];
            isoo.read(buffer, 4);
            int len = bytesToInt(buffer);
            char* bufTopic = new char[len];
            isoo.read(bufTopic, len);
            std::string topic{ bufTopic,(uint32_t)len };
            int dataLen = (int)buf.size()- 4 - len;
            char* data = new char[dataLen];
            isoo.read(data, dataLen);
            auto msg = ConvertToMsgByte(data, dataLen);
            if (recmsgtopic != nullptr)
            {
                recmsgtopic(topic, msg, dataLen);
            }
            if (callback != nullptr)
            {
                callback(topic, msg, dataLen);
            }
           
           
        }
    }
    void IpcNative::ipcRecv()
    {

        if (isStart)
        {
            return;
        }
        ReceiveData();
    }
	void IpcNative::ipcSend(string topic, char buf[],int len)
	{
       
            
             const char* bufTopic = topic.c_str();
             int size = (int)topic.size();
             int dataLen = 4 + len + size;
             char* topicLen = new char[4];
             char* sum = new char[dataLen];
             intToByte(size, topicLen);
             std::ostringstream osoo(dataLen);
             osoo.write(topicLen, 4);
             osoo.write(bufTopic, size);
             osoo.write(buf, len);
       
              string ss = osoo.str();
              const char* buffer = ss.c_str();
              osoo << sum;
           
              sender_.send(buffer, dataLen);
              osoo.clear();

	}
}
