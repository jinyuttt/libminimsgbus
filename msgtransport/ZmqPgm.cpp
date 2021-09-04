
#include "ZmqPgm.h"
#include <thread>
#include "zmq.hpp"
#include "zmq_addon.hpp"
using namespace msgtransport;
using namespace std;
namespace  msgtransport
{
    ZmqPgm::ZmqPgm()
    {
        zmq::socket_t pub(ctx, zmq::socket_type::pub);
        zmq::socket_t sub(ctx, zmq::socket_type::sub);
        pubSocket.swap(pub);
        subSocket.swap(sub);
    }
    ZmqPgm::~ZmqPgm()
    {
      
    }
	void ZmqPgm::ReceiveData()
	{
        std::lock_guard<std::mutex> l(sigsocket);
        if (isStart)
        {
            return;
        }
        isStart = true;
        thread rectopic(&ZmqPgm::ThreadReceive,this);
        rectopic.detach();
     
	}
	
    void ZmqPgm::ThreadReceive()
    {
        while (true)
        {
            std::vector<zmq::message_t> recv_msgs;
            zmq::recv_result_t result =
                zmq::recv_multipart(subSocket, std::back_inserter(recv_msgs));
            string topic = recv_msgs[0].to_string();
            const char* p = (char*)recv_msgs[1].data();
            int len = recv_msgs[1].size();
            auto msg = ConvertToMsgByte(p, len);

            if (callback != nullptr)
            {
                callback(topic, msg, len);
            }
        }
    }
    
    void ZmqPgm::subscribe(string topic)
	{
        if (isSubBind)
        {
            if (!LocalAddres.empty())
            {
                for (auto p:LocalAddres)
                {
                    string addr = "epgm://" + p + ";" + MultAddress;
                  
                    try
                    {
                        subSocket.connect(addr);
                    }
                    catch (const std::exception&e)
                    {
                        std::cout << e.what() << std::endl;
                    }
                  
                }
            }
           subSocket.set(zmq::sockopt::linger, 1000);
            isSubBind = false;
        }
        //
        if (topic.empty())
        {
            subSocket.set(zmq::sockopt::subscribe, "");
        }
        else
        {
            subSocket.set(zmq::sockopt::subscribe, topic);
        }
        if (isStart)
        {
            return;
        }
       
         ReceiveData();
        
	}
	void ZmqPgm::publish(string topic, char buf[])
	{
        if (isBind)
        {
            if (!LocalAddres.empty())
            {
                for(auto p : LocalAddres)
                {
                    string addr = "epgm://" + p + ";" + MultAddress;
                    pubSocket.bind(addr);
                }
            }
            pubSocket.set(zmq::sockopt::linger, 1000);
            isBind = false;
        }
        int size =strlen(buf);
        pubSocket.send(zmq::const_buffer(topic.c_str(),topic.size()), zmq::send_flags::sndmore);//这里定一个主题
        pubSocket.send(zmq::const_buffer(buf, size));//主题数据，只使用数据
     

        
	}
}