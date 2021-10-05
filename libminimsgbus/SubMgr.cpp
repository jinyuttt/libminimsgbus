
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

    void SubMgr::addQueue(TopicStruct data)
    {
        std::lock_guard<std::mutex> guard(data_mutex);
        queuqdata.push(data);
    }

    bool SubMgr::getQueueData(TopicStruct& item)
    {
        std::lock_guard<std::mutex> guard(data_mutex);
        if (queuqdata.empty())
        {
            return false;
        }
        auto p = queuqdata.front();
        item = p;
        queuqdata.pop();
        return true;
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

        auto ov = LocalTopic::GetLocal(topic);
        if (ov != nullptr)
        {
            SubMgr::GetInstance()->sendSub(topic, ov);
        }
    

    }

    void SubMgr::processSub()
    {

        thread queue([&]()
            {
           
                while (true)
                {
                    TopicStruct p;
                   
                    auto hv = topicStructs.wait_dequeue_timed(p, waitTime);
                
                
                    if (!hv)
                    {
                        continue;
                    }
                 
                    if (p.Flage == 1)
                    {
                      
                        //���ĵ�ַ����
                        SubTable::GetInstance()->add(p.Topic, p.Msg, p.MsgNode);
                    }
                    else
                    {
                       
                        if (dicMsg.find(p.MsgNode + to_string(p.MsgId)) != dicMsg.end())
                        {
                            return;
                        }
                        auto tp = std::chrono::steady_clock::now();
                        dicMsg[p.MsgNode + to_string(p.MsgId)] = tp;
                      
                        //���ݴ���
                        thread th([=]()
                            {

                                auto lstitor = dicSubObj.find(p.Topic);
                                if (lstitor != dicSubObj.end())
                                {
                                    for (auto& lst : (lstitor->second))
                                    {
                                        lst->addtopic(p.Topic, p.Msg, p.msglen);
                                    }
                                }
                            });
                        th.detach();
                    }
                }
               
            });
        queue.detach();



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
                    else
                    {

                        // ���ĵ�ַ����
                        SubTable::GetInstance()->add(v.Topic, string(v.Msg,v.msglen), v.MsgNode);

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
        auto lst = PubTable::GetInstance()->getAddress(topic);
        auto remote = MsgLocalNode::remote;
        NngDataNative nng;
        if (lst.empty())
        {
            //û�з�����ַ�����뱾�ؽڵ���Ϣ
            std::cout << "��ʱ���뱾�أ�" + topic<<std::endl;
            
            LocalTopic::AddLocal(topic, sub);
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
      
        for (auto pub : lst)
        {
           
            //��������ΪInitSub�����ȳ�ʼ��
            //�������е�ַ����Ҫ����ȷ�ĵ�ַ���ͳ�ȥ����
            //ȡ����ʵ�Ķ˿�
            for (auto p : TopicBroadcast::lstNodeAddress)
            {
                int len = 0;
                char* data = nullptr;
                data=  Util::ConvertString(p,len);
                auto v = Util::Convert(topic, data, len, '1', 0, len);
                nng.send(p, v, len);
                std::cout << "���Ͷ��� topic:" << topic << "  addr:" << pub << std::endl;
            }

        }
        if (sub == nullptr)
        {
            //û�ж��Ļ����Ѿ����붩��
            return;
        }
        //���ֱ��ض���ʵ�����������ݻش�
        auto itor = dicSubObj.find(topic);
        if (itor != dicSubObj.end())
        {
            auto& v = itor->second;
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
        }

    }


};

