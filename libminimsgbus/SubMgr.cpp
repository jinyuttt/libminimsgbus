
#include "SubMgr.h"

namespace libminimsgbus
{
    SubMgr::SubMgr()
    {
        topicBroadcast = new  TopicBroadcast();
       
        Init();
    }

    SubMgr::~SubMgr()
    {
        delete topicBroadcast;
    }
    void SubMgr::openChanel()
    {
    }

    void SubMgr::Init()
    {
        for (int i = 0;i <= MsgLocalNode::GUID.length();i++)
            Util::guid[i] = MsgLocalNode::GUID[i];
        initDataRecive();
        initPgm();
        processSub();
        removeFilter();
    }

   
    void SubMgr::removeSubscriber(string topic)
    {
        std::lock_guard<std::mutex> lck(obj_mutx);
       auto l_it = dicSubObj.find(topic);
        if (l_it != dicSubObj.end())
                dicSubObj.erase(l_it); 
    }

    list<msgtopic*> SubMgr::getSubscriber(string topic)
    {
        std::lock_guard<std::mutex> lck(obj_mutx);
        auto litor = dicSubObj.find(topic);
        if (litor != dicSubObj.end())
        {
            return  litor->second;
        }
        return list<msgtopic*>();
    }


    void SubMgr::initPgm()
    {

        topicBroadcast->revTopic = TopicBroadcast_ReceiveTopic;
        topicBroadcast->topicSub();
        //
        topicBroadcast->pgmPub("Global");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    void SubMgr::getRealAddress(string address, string& protol, string& ip, int& port)
    {


        auto index = address.find("//");
        auto index1 = address.find_last_of(":");

        if (index != address.npos)
        {
            protol = address.substr(0, index - 1);
        }
        ip = address.substr(index + 2, index1 - index - 2);
        port = std::stoi(address.substr(index1 + 1));
        //Console.WriteLine(string.Format("ͨ��Э��:{0} ��IP:{1} �󶨶˿�:{2}", protol, ip, port));
        return;

    }

   
    void SubMgr::TopicBroadcast_ReceiveTopic(string topic, string address)
    {
        //���·����ڵ���뱾��
        PubTable::GetInstance()->add(topic, address);
        std::cout << "�¼���ķ�����ַ������:" << topic << "��ַ:" << address << std::endl;
      
        //�鿴���ڵ��Ƿ��Ѿ����Ĺ��������

        auto ov = LocalTopic::getLocal(topic);
        if (ov != nullptr)
        {
            SubMgr::GetInstance()->sendSub(topic, ov);
        }
    }

    void SubMgr::processSub()
    {

           thread queuethread([&]()
            {
           
                while (true)
                {
                    TopicStruct p;
                   
                    auto hv = topicStructs.wait_dequeue_timed(p, waitTime);
                    if (!hv)
                    {
                        continue;
                    }
                        if (dicMsg.find(p.MsgNode + to_string(p.MsgId)) != dicMsg.end())
                        {
                            return;
                        }
                        auto tp = std::chrono::steady_clock::now();
                        dicMsg[p.MsgNode + to_string(p.MsgId)] = tp;
                        //��������
                        string topic = p.Topic;
                        const string&data = string(p.Msg, p.msglen);
                        //���ݴ���
                        thread handthread(&SubMgr::handData,this, p.Topic,data);
                        handthread.detach();
                        delete p.Msg;
                }
               
            });
        queuethread.detach();
    }

    void SubMgr::handData(string topic, const string& data)
    {
        auto lstitor = getSubscriber(topic);
        auto msg =const_cast<char*>(data.data());
        int len = data.length();
        try
        {
            for (auto lst : lstitor)
            {
                //����msgtopic
                if (lst != nullptr)
                    lst->addtopic(topic, msg, len);
            }
        }
        catch (std::exception e)
        {
            std::cout << e.what() << std::endl;
        }
        //delete msg;
    }

    void SubMgr::initDataRecive()
    {

        
        string tmp = "";

        if (MsgLocalNode::LocalAddress == "")
        {

            if (MsgLocalNode::protocol == "")
            {
             
                tmp = "*:" + to_string(MsgLocalNode::LocalPort);
            }
            else
            {
               
                tmp = MsgLocalNode::protocol + "://*:" + to_string(MsgLocalNode::LocalPort);
            }
            MsgLocalNode::Netprotocol = native->receive(tmp);
        }
        else
        {
            if (MsgLocalNode::protocol == "")
            {
               
                tmp = MsgLocalNode::LocalAddress + ":" + to_string(MsgLocalNode::LocalPort);
            }
            else
            {
               
                tmp = MsgLocalNode::protocol + "://" + MsgLocalNode::LocalAddress + ":" + to_string(MsgLocalNode::LocalPort);
            }
            MsgLocalNode::Netprotocol = native->receive(tmp);
        }
        //
        string protol;
        string ip;
        int port = 0;
        getRealAddress(MsgLocalNode::Netprotocol, protol, ip, port);
        MsgLocalNode::LocalAddress = ip;
        MsgLocalNode::LocalPort = port;
        MsgLocalNode::protocol = protol;
        std::cout << "LocalNode.LocalAddress:" + MsgLocalNode::LocalAddress << std::endl;
        std::cout << "LocalNode.port::" + MsgLocalNode::LocalPort << std::endl;
      
        topicBroadcast->getLocalAddress();//��ʼ��һ�ε�ַ
        thread rec([&]()
            {
                //��������
             
                while (true)
                {
                    auto buf = native->getData();

                    auto v = Util::Convert(buf.bufdata,buf.size);

                    if (v.Flage == '0')
                    {
                     
                        topicStructs.enqueue(v);

                    }
                    if (v.Flage == '1')
                    {
                        // ���ĵ�ַ����
                        SubTable::GetInstance()->add(v.Topic, string(v.Msg, v.msglen), v.MsgNode);
                        delete v.Msg;
                    }
                    else if (v.Flage == '2')
                    {
                        SubTable::GetInstance()->remove(v.Topic, v.MsgNode);
                        delete v.Msg;
                    }
                   
                   delete buf.bufdata;
                }
            });
        rec.detach();



    }

    void SubMgr::removeFilter()
    {
        {
            thread thread([&]()
                {
                    while (true)
                    {

                        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); //����150����
                        list<string> lst;

                        for (auto p : dicMsg)
                        {
                            lst.push_back(p.first);
                        }


                        for (auto p : lst)
                        {
                            auto itor = dicMsg.find(p);
                            if (itor != dicMsg.end())
                            {
                                //������2������ݣ���Ϊ�ظ������ݲ��ᳬ��2��
                              //�ظ���ԭ���ǽڵ�࿨��
                                auto tp = std::chrono::steady_clock::now();
                                //auto tsp = std::chrono::duration_cast<std::chrono::microseconds>(tp - (itor->second));
                                double duration_millsecond = std::chrono::duration<double, std::micro>(tp - (itor->second)).count();

                                if (duration_millsecond > 2 * 1e6)
                                {
                                    dicMsg.erase(itor);
                                }
                                //duration<double> time_span = duration_cast<duration<double>>(tp - (itor->second));
                                /*if (tp - itor->second > 2 * 1e9)
                                {
                                    dicMsg.TryRemove(p, out ticks);
                                }*/
                            }

                        }
                    }
                });
            thread.detach();
        }
    }

    void SubMgr::sendSub(string topic, msgtopic* sub)
    {
      
        std::cout << "�������ⶩ����Ϣ:" + topic << std::endl;
        //���ֱ��ض���ʵ�����������ݻش�
        if (sub == nullptr)
        {
            //û�ж��Ļ����Ѿ����붩��
            return;
        }
        auto lst = PubTable::GetInstance()->getAddress(topic);
        auto remote = MsgLocalNode::remote;
        bool isSend = false;
        if (lst.empty())
        {
            //û�з�����ַ�����뱾�ؽڵ���Ϣ
            std::cout << "��ʱ���뱾�أ�" + topic<<std::endl;
            LocalTopic::addLocal(topic, sub);
           if(remote.empty())
            return;
        }
        
        if (!remote.empty())
        {
            //������ַ
            for (auto it : remote)
            {
                lst.push_back(it);
            }
        }
        //
      
        auto itor = dicSubObj.find(topic);
        if (itor != dicSubObj.end())
        {
            auto v = itor->second;
            for (auto p : v)
            {
                if (p == sub)
                {
                    return;
                }
            }
            itor->second.push_back(sub);
        }

        else
        {
            list<msgtopic*> lstTopic;
            lstTopic.push_back(sub);
            dicSubObj[topic] = lstTopic;
            isSend = true;//��һ�νڵ㶩������
        }

        if (isSend)
        {
            for (auto pub : lst)
            {
                NngDataNative nng;
                //��������ΪInitSub�����ȳ�ʼ��
                //�������е�ַ����Ҫ����ȷ�ĵ�ַ���ͳ�ȥ����
                //ȡ����ʵ�Ķ˿�
                for (auto p : TopicBroadcast::lstNodeAddress)
                {
                    int len = 0;
                    char* data = nullptr;
                    data = Util::ConvertString(p, len);
                    auto v = Util::Convert(topic, data, len, '1', 0, len);
                    nng.send(p, v, len);
                    std::cout << "���Ͷ��� topic:" << topic << "  addr:" << pub << std::endl;
                }

            }
        }
        
        

    }

    void SubMgr::sendUnsub(string topic)
    {
       auto lst= PubTable::GetInstance()->getAddress(topic);
       if (!lst.empty())
       {

           NngDataNative nng;
          
           removeSubscriber(topic);
           for (auto& p : lst)
           {
               int msglen;

               auto buf = Util::Convert(topic, "-1", 1, '2', -1, msglen);
               auto ret = nng.send(p, buf, msglen);
              

           }
       }
    }

    void SubMgr::eraseBus(msgtopic* topic)
    {
        std::lock_guard<std::mutex> lck(obj_mutx);
        auto l_it = dicSubObj.begin();
        while (l_it != dicSubObj.end())
        {
            auto itor = l_it->second.begin();
            while (itor != l_it->second.end())
            {
                if (*itor == topic)
                {
                    l_it->second.erase(itor++);
                    break;
                }
            }
            l_it++;
        } 
           
    }


};

