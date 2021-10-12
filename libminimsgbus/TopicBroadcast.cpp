
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
            //�󶨱�������IP
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
        auto tmp = Util::StringSplit(obj, "[\\s|?]+");//�����뷢����ַ��|�ָ�
        string builder;
        for (int i = 0; i < tmp.size() - 1; i++)
        {
            //��ֹ��������|
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
        //    return;//�Լ��Ĳ�Ҫ
        //}
        if (topic == "Global")
        {
            broadcast(false);//ipc������ipc�ظ�
            return;
        }
        auto msg = address.substr(0, address.size() - MsgLocalNode::GUID.size() - 2);
        std::cout << "ipc��Ϣ;"<< msg << std::endl;
        if (revTopic != nullptr)
        {
            revTopic(topic, msg);
        }
    }

    void TopicBroadcast::pgm_ReceiveTopic(string arg1, char* arg,int len)
    {

       
        std::cout << "pgm����" << std::endl;
        auto data = string(arg);
        string topic;
        string address;
        getTopicData(data, topic, address);
        if (topic == "Global")
        {
            broadcast(true);//�鲥�������鲥�ظ�
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
        //�������������������
        //�󶨱�������IP
        if (pgm->LocalAddres.empty())
        {
            pgm->LocalAddres = lstBindIP;
        }

        for (auto p : lstNodeAddress)
        {
            try
            {
                //֪ͨ���ڵ㷢�����ⷢ����ַ���鲥��ipcͬʱ���鲥���ܱ�ռ
                string pgmtmp = (topic + "|" + p);
                string ipctmp = topic + "|" + p + ">>" + MsgLocalNode::GUID;
                int pgmlen = 0;
                int ipclen = 0;
                auto buf = Util::ConvertString(pgmtmp, pgmlen);
                auto bufa = Util::ConvertString(ipctmp, ipclen);
                pgm->publish("noticetopicaddress", buf, pgmlen);
                ipc->ipcSend("minimsg", bufa, ipclen);//ͬʱ�������������ڵ�
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


        //������յ�����Global��Ϣ����ѱ��ڵ㱣�ֵ����з����ڵ㷢�ͳ�ȥ�����¼���Ľڵ��ȡ
        //������ַ
        auto dic = PubTable::GetInstance()->getPairs();
       
        std::cout << "���յ�Global���⣬֪ͨһ��ȫ�������ַ" << std::endl;
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
                    //����̶���һ�������ʽ
                    string ipctmp = kv.first + "|" + p + ">>" + MsgLocalNode::GUID;
                    auto bufa = Util::ConvertString(ipctmp,len);
                    ipc->ipcSend("minimsg", bufa,len);
                }

            }

        }



    }
}