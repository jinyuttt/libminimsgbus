
#include "PubMgr.h"
#include <NngDataNative.h>
#include "SubMgr.h"
namespace libminimsgbus
{
    PubMgr::PubMgr()
    {
        topicBroadcast = new TopicBroadcast();
        _semaphore.resize(std::thread::hardware_concurrency());
        _sub.resize(std::thread::hardware_concurrency());
        errorRecords = new BlockingConcurrentQueue<Records>();
        init();
        checkErrRecord();
    }
    void PubMgr::init()
    {
        //strcpy_s(Util::guid, MsgLocalNode::GUID.data());//����ʱ��ʼ��
        for (int i = 0;i <= MsgLocalNode::GUID.length();i++)
            Util::guid[i] = MsgLocalNode::GUID[i];

    }
    void	PubMgr::checkErrRecord()
    {

        thread check([&]()
            {
                int num = std::thread::hardware_concurrency();
                for (int i = 0;i < num; i++)
                {
                    // ��ʼ��
                    ErrorQueue tmp;
                    lstError.push_back(new ErrorQueue);

                }
                // ÿ�����⣬ѭ������ÿ�����ⷢ��ʧ�ܵ�����
                while (true)
                {
                    Records p;
                    errorRecords->wait_dequeue(p);
                    auto cur = recordsProcess(p);
                    if (cur->IsComlepte)
                    {
                        _semaphore.wait();//�ȴ�ǰ������⴦����ɣ�����̫���߳�
                        startThreadRecord(*cur);
                    }

                }
            });
        check.detach();
    }

    ErrorQueue* PubMgr::recordsProcess(Records queue)
    {


        ErrorQueue* cur=nullptr;

        for (auto pl = lstError.begin(); pl != lstError.end();++pl) {
            cur = *pl;
            if (cur->Topic == queue.Topic)
            {
                break;
            }
        }

        if (cur == nullptr)
        {
            for (auto pl = lstError.begin(); pl != lstError.end();++pl) {
                cur = *pl;
                if (cur->IsComlepte)
                {
                    break;
                }
            }

            cur->queueError.enqueue(queue);
            cur->Topic = queue.Topic;

        }
        else
        {

            cur->queueError.enqueue(queue);

        }
        return cur;


    }

    void PubMgr::startThreadRecord(ErrorQueue& queue)
    {

        thread t([&]()
            {
                queue.IsComlepte = false;
                Records p;
                bool isComp = false;
                while (!isComp)
                {

                    queue.queueError.try_enqueue(p);
                    isComp = true;
                    for (auto &kv : p.Record)
                    {
                        isComp = false;
                        //�������ٴη���
                        NngDataNative nng;
                        int64_t msgid = 0;
                        int dlen = 0;
                        auto buf = Util::Convert(p.Topic, kv.second, strlen(kv.second), '0', msgid, dlen);
                        int len = 0;
                        auto ret = nng.send(kv.first, buf, &len);
                      
                        if (ret == nullptr || len == 0)
                        {
                            //��ȡ���ض��Ľڵ�
                            auto addr = SubTable::GetInstance()->getAddressLst(p.Topic);
                          
                              //����ʧ�ܣ��ж��Ƿ��Ƕ������󶨶��ĵ�ַ
                              //�뵱ǰ�ڵ�ʹ�õ�ͨ�ŵ�ַһ�µĵ�ַ��Ϣ
                            SubAddress err;
                            for (auto &my : addr.SubAddresses)
                            {
                                if (my.Address == kv.first)
                                {
                                    err = my;
                                    break;
                                }
                            }
                            if (err.Address == "")
                            {
                                //û���ҵ�������ǰ���Ѿ��滻��
                               // string node = "";
                                auto node = dicNodeGuid.find(kv.first);
                                if (node != dicNodeGuid.end())
                                {
                                    for (auto &my : addr.SubAddresses)
                                    {
                                        if (my.NodeFlage == node->second)
                                        {
                                            err = my;
                                            break;
                                        }
                                    }

                                }
                            }
                            else
                            {
                                //�����жϷ��쳣�ĵ�ַ��ͬһ�ڵ�
                                list<string> cur;
                                bool isfind = false;
                                for (auto &addtmp : err.AllAddress)
                                {
                                    for (auto &myaddr : err.ErrorAddress)
                                    {
                                        if (addtmp == myaddr)
                                        {
                                            isfind = true;
                                            break;
                                        }
                                    }
                                    //
                                    if (!isfind)
                                    {
                                        cur.push_back(addtmp);
                                    }
                                }
                                //
                                if (cur.empty())
                                {
                                    //�Ѿ�û����ȷ��ַ,�ڵ����е�ַ������2�κ��Ƴ��ڵ㣬�����Ľڵ��쳣
                                     //�����������ݷ���2��
                                    if (err.NumAll > 1)
                                    {
                                        for (auto it = addr.SubAddresses.begin(); it != addr.SubAddresses.end(); ) {
                                            if (it->NodeFlage == err.NodeFlage)
                                            {
                                                addr.SubAddresses.erase(it);
                                                break;
                                            }
                                        }

                                        for (auto it = addr.LstAddress.begin(); it != addr.LstAddress.end(); ) {
                                            for (auto &atmp : err.AllAddress)
                                            {
                                                if (atmp == *it)
                                                {
                                                    addr.LstAddress.erase(it++);
                                                    break;
                                                }
                                            }
                                        }


                                        for (auto &addrerr : err.AllAddress)
                                        {
                                            //���õ�������
                                            auto tmp = dicNodeGuid.find(addrerr);
                                            if (tmp != dicNodeGuid.end())
                                            {
                                                dicNodeGuid.erase(tmp);
                                            }
                                        }
                                        err.NumAll = 0;
                                        /* if (LocalNode.IsMsgReturn)
                                         {
                                             MsgTopicCount.Instance.Add(new PubRecords(){ FaildNum = 1, MsgId = p.MsgId });
                                         }*/
                                         // Console.WriteLine("�쳣����ڵ�û�н����,{0}>>{1}>>", kv.Key, addr.SubAddresses.Count);
                                        continue;
                                    }
                                    cur = err.AllAddress;//ȫ����
                                    err.NumAll++;
                                }

                                //
                                for (auto right : cur)
                                {
                                    int dlen = 0;
                                    auto buf = Util::Convert(p.Topic, kv.second,strlen(kv.second), '0', msgid, dlen);
                                    int len = 0;
                                    auto ret = nng.send(right, buf, &len);

                                    if (ret != nullptr && len > 0)
                                    {
                                        //�յ��ظ��õ�ַ����
                                        err.ErrorAddress.push_back(err.Address);
                                        err.Address = right;
                                        dicNodeGuid[kv.first] = err.NodeFlage;//����ǰ���������ĵ�ַ����
                                       /* Console.WriteLine("�ɹ������쳣��" + right);
                                        if (LocalNode.IsMsgReturn)
                                        {
                                            MsgTopicCount.Instance.Add(new PubRecords(){ SucessNum = 1, MsgId = p.MsgId });
                                        }*/
                                        break;
                                    }
                                    else
                                    {
                                        //�ڵ��쳣��ַ����
                                        err.ErrorAddress.push_back(right);
                                    }
                                }
                            }

                        }
                    }

                }
                queue.IsComlepte = true;
                _semaphore.signal();
            });
    }

    uint64_t PubMgr::send(string topic, char msg[])
    {
     
        int lenm = 0;
    
        //��¼�����쳣�ĵ�ַ������
        map<string, char*> dic ;

        //�ӱ����Ѿ����ĵĵ�ַ����
        auto lst = SubTable::GetInstance()->getAddress(topic);
        int64_t id = msgid.fetch_add(1);
        if (!lst.empty())
        {

            NngDataNative nng;
            PubRecords records{ 0,0,0 };
            for (auto &p : lst)
            {
                int len;
                int m = sizeof(msg);
                int mm = strlen(msg);
                auto buf = Util::Convert(topic, msg,strlen(msg), '0', id, len);
                auto ret = nng.send(p, buf, &len);
                if (ret == nullptr || len == 0)
                {
                    //����ʧ��û�з���
                  
                    dic[p] = msg;
                    records.FaildNum++;
                }
                else
                {
                    records.SucessNum++;
                }

            }
            if (!dic.empty())
            {
                //�����쳣���ݣ��ȴ��ٴδ���
                Records tmp{ dic,topic,msgid };
                errorRecords->enqueue(tmp);
                //MsgTopicCount.Instance.AddTemp(records);
            }
            else
            {
                // MsgTopicCount.Instance.Add(records);
            }
          //  delete nng;
            return id;
        }
        else
        {

            //����û�ж��Ľڵ�
            auto lstPub = PubTable::GetInstance()->getAddress(topic);//��ȫ�ַ������в�ѯ
            if (!lstPub.empty())
            {
                //���ڵ��Ѿ���������ַ�Ͷ�����,˵��û�нڵ㶩���������

                string find;
                for (auto &tmp : lstPub)
                {
                    for (auto &tmpaddr : TopicBroadcast::lstNodeAddress)
                    {
                        if (tmp == tmpaddr)
                        {
                            return id;
                        }
                    }
                }

            }
            SubMgr::GetInstance()->openChanel();//��ʼ�����������ݽ��ն���
            //                             //��һ�α��ڵ㷢��
            auto lstLocal = topicBroadcast->pgmPub(topic);

            //���·����ڵ���뱾��
            for (auto p : lstLocal)
            {
                
               // std::cout << "���ڵ���뷢����ַ:" + p << std::endl;
                PubTable::GetInstance()->add(topic, p);
            }
         
            _sub.wait();
         
            thread th([=]()
                {

                    //�ȴ�20�Σ�ÿ��100ms,1������ɷ������������ݶ�ʧ����Ϊ��ַ֪ͨ����û�лظ�
                    //֪ͨ��ַ�󲻻�����Ϣ�ظ�����������Ϣ������
                    for (int i = 0; i < 10; i++)
                    {
                        //  Thread.Sleep(100);//�ȴ�100msȡ���ĵ�ַ
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        //�ٴμ���Ƿ��ж���;
                        PubRecords records{ 0,0,0 };
                        auto lst = SubTable::GetInstance()->getAddress(topic);
                        if (!lst.empty())
                        {
                            for (auto p : lst)
                            {
                               
                                int len = 0;
                                auto buf = Util::Convert(topic, msg, strlen(msg), '0', id, len);
                                NngDataNative nng;
                                auto ret = nng.send(p, buf, &len);
                                if (ret == nullptr)
                                {
                                    records.FaildNum++;
                                }
                                else
                                {
                                    records.SucessNum++;
                                }
                            }
                            //if (LocalNode.IsMsgReturn)
                            //{
                            //  //  MsgTopicCount.Instance.Add(records);
                            //}
                            break;
                        }
                    }
                
                    _sub.signal();
                });
            th.detach();
            return id;
        }
    };


}
