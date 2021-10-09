
#include "ZmqPgm.h"
#include <thread>
#include <zmq_addon.hpp>
#include <zmq.h>
using namespace msgtransport;
using namespace std;
namespace  msgtransport
{
    ZmqPgm::ZmqPgm()
    {
      
        pubSocket = new zmq::socket_t(ctx, zmq::socket_type::pub);
        subSocket=new  zmq::socket_t(ctx, zmq::socket_type::sub);
    }
    ZmqPgm::~ZmqPgm()
    {
        isRun = false;
        delete subSocket;
        delete pubSocket;
      
    }
	void ZmqPgm::receiveData()
	{
        std::lock_guard<std::mutex> l(sigsocket);
        if (isStart)
        {
            return;
        }
        isStart = true;
        thread rectopic(&ZmqPgm::threadReceive,this);
        rectopic.detach();
     
	}
	
    void ZmqPgm::threadReceive()
    {
        while (isRun)
        {
            std::vector<zmq::message_t> recv_msgs;
            zmq::recv_result_t result =
                zmq::recv_multipart(*subSocket, std::back_inserter(recv_msgs));
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
                        subSocket->connect(addr);
                    }
                    catch (const std::exception&e)
                    {
                        std::cout << e.what() << std::endl;
                    }
                  
                }
            }
           subSocket->set(zmq::sockopt::linger, 1000);
            isSubBind = false;
        }
        //
        if (topic.empty())
        {
            subSocket->set(zmq::sockopt::subscribe, "");
        }
        else
        {
            subSocket->set(zmq::sockopt::subscribe, topic);
        }
        if (isStart)
        {
            return;
        }
       
         receiveData();
        
	}

	void ZmqPgm::publish(string topic, char buf[],int len)
	{
        if (isBind)
        {
            if (!LocalAddres.empty())
            {
                for(auto p : LocalAddres)
                {
                    string addr = "epgm://" + p + ";" + MultAddress;
                    pubSocket->bind(addr);
                }
            }
            pubSocket->set(zmq::sockopt::linger, 1000);
            isBind = false;
        }
        int size = len;
        pubSocket->send(zmq::const_buffer(topic.c_str(),topic.size()), zmq::send_flags::sndmore);//这里定一个主题
        pubSocket->send(zmq::const_buffer(buf, size));//主题数据，只使用数据
     

        
	}

    void ZmqPgm::allClose()
    {
        //zsys_shutdown();
        //pubSocket->close();
        //subSocket->close();
        zmq_ctx_term(&ctx);
        zmq_ctx_destroy(&ctx);

    }
}