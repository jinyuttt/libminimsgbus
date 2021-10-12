
#include "TopicBroadcast.h"
namespace libminimsgbus
{
    list<string> TopicBroadcast::lstNodeAddress;
    list<string> TopicBroadcast::lstBindIP;
     std::mutex TopicBroadcast::mtx;
    TopicBroadcast::TopicBroadcast()
    {
        pgm = new ZmqPgm();
        ipc = new IpcNative();
        initAddress();
    }
    TopicBroadcast::~TopicBroadcast()
    {
        pgm->allClose();
        delete pgm;
        delete ipc;
    }
    void TopicBroadcast::initAddress()
    {
        std::lock_guard<std::mutex> sbguard1(mtx);
        auto find = MsgLocalNode::LocalAddress.find("*");
        if (find != string::npos)
        {
            //绑定本地所有IP
            MsgLocalNode::GetNetworkInterface();

            for (auto p : MsgLocalNode::LocalAddressFamily)
            {
                if (p.IPV4 == "127.0.0.1")
                {
                    continue;
                }
                lstBindIP.push_back(p.IPV4);
            }
        }
        else
        {
            lstBindIP.push_back(MsgLocalNode::LocalAddress);
        }

    }
    
    void TopicBroadcast::getLocalAddress()
    {

        for (auto p : lstBindIP)
        {

            string tmp = "";
            if (MsgLocalNode::protocol == "")
            {
              
                tmp = p + ":" + to_string(MsgLocalNode::LocalPort);
            }
            else
            {
                
                tmp = MsgLocalNode::protocol + "://" + p + ":" + to_string(MsgLocalNode::LocalPort);
            }
            lstNodeAddress.push_back(tmp);
         
        }
        if (!lstNodeAddress.empty())
        {
            std::set<string> settmp;
            for (auto str : lstNodeAddress)
            {
                settmp.insert(str);
            }
            lstNodeAddress.clear();
            for (auto str : settmp)
            {
                lstNodeAddress.push_back(str);
            }
        }
       
    }
   
    void TopicBroadcast::getTopicData(string obj, string& topic, string& address)
    {
        auto tmp = Util::StringSplit(obj, "[\\s|?]+");//主题与发布地址用|分割
        string builder;
        for (int i = 0; i < tmp.size() - 1; i++)
        {
            //防止主题中有|
            builder.append(tmp[i] + "|");
        }
        builder.resize(builder.size() - 1);
        topic = builder;
        address = tmp[tmp.size() - 1];

    }
    
    void TopicBroadcast::topicSub()
    {

        //
        if (pgm->LocalAddres.empty())
        {
            pgm->LocalAddres = lstBindIP;
        }
     
        pgm->callback = std::bind(&TopicBroadcast::pgm_ReceiveTopic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        pgm->subscribe("noticetopicaddress");
        ipc->callback= std::bind(&TopicBroadcast::ipc_ReceiveTopic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);;
        ipc->ipcRecv();


    }

    void TopicBroadcast::ipc_ReceiveTopic(string arg1, char * arg,int dlen)
    {
        auto data = string(arg,dlen);
        string topic;
        string address;
        getTopicData(data, topic, address);
        int len = address.size();
        int flageLen = MsgLocalNode::GUID.size();
        //auto guid = address.substr(len - flageLen);
        //if (guid == MsgLocalNode::GUID)
        //{
        //    return;//自己的不要
        //}
        if (topic == "Global")
        {
            broadcast(false);//ipc接收用ipc回复
            return;
        }
        auto msg = address.substr(0, address.size() - MsgLocalNode::GUID.size() - 2);
        std::cout << "ipc消息;"<< msg << std::endl;
        if (revTopic != nullptr)
        {
            revTopic(topic, msg);
        }
    }

    void TopicBroadcast::pgm_ReceiveTopic(string arg1, char* arg,int len)
    {

       
        std::cout << "pgm接收" << std::endl;
        auto data = string(arg);
        string topic;
        string address;
        getTopicData(data, topic, address);
        if (topic == "Global")
        {
            broadcast(true);//组播接收用组播回复
            return;
        }
        if (revTopic != nullptr)
        {
            revTopic(topic, address);
        }
    }

    list<string> TopicBroadcast::pgmPub(string topic)
    {

        list<string> lst;
        //如果绑定了所有网卡接收
        //绑定本地所有IP
        if (pgm->LocalAddres.empty())
        {
            pgm->LocalAddres = lstBindIP;
        }

        for (auto p : lstNodeAddress)
        {
            try
            {
                //通知本节点发布主题发布地址，组播和ipc同时，组播可能被占
                string pgmtmp = (topic + "|" + p);
                string ipctmp = topic + "|" + p + ">>" + MsgLocalNode::GUID;
                int pgmlen = 0;
                int ipclen = 0;
                auto buf = Util::ConvertString(pgmtmp, pgmlen);
                auto bufa = Util::ConvertString(ipctmp, ipclen);
                pgm->publish("noticetopicaddress", buf, pgmlen);
                ipc->ipcSend("minimsg", bufa, ipclen);//同时发给本机其它节点
            }
            catch (std::exception ex)
            {
               
                std::cout << ex.what() << std::endl;
            }

        }
        return lstNodeAddress;

    }

    void TopicBroadcast::broadcast(bool ispgm)
    {


        //如果接收到的是Global信息，则把本节点保持的所有发布节点发送出去，让新加入的节点获取
        //发布地址
        auto dic = PubTable::GetInstance()->getPairs();
       
        std::cout << "接收到Global主题，通知一次全局主题地址" << std::endl;
        for (auto kv : dic)
        {
            for (auto p : kv.second)
            {
                if (pgm->LocalAddres.empty())
                {
                    pgm->LocalAddres = lstBindIP;
                }
                if (ispgm)
                {
                    string pgmtmp = (kv.first + "|" + p);
                    int len = 0;
                   auto buf = Util::ConvertString(pgmtmp,len);

                   pgm->publish("noticetopicaddress", buf,len);
                }
                else
                {
                    int len = 0;
                    //这里固定了一个特殊格式
                    string ipctmp = kv.first + "|" + p + ">>" + MsgLocalNode::GUID;
                    auto bufa = Util::ConvertString(ipctmp,len);
                    ipc->ipcSend("minimsg", bufa,len);
                }

            }

        }



    }
}