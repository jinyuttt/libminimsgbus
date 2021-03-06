
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
        errorRecords = new BlockingConcurrentQueue<Records>(numeric_limits<int> ::max(),1,10);
        init();
        checkErrRecord();
    }

    void PubMgr::init()
    {
     
        for (int i = 0;i <= MsgLocalNode::GUID.length();i++)
            Util::guid[i] = MsgLocalNode::GUID[i];

    }

    list<FirstTopic>  PubMgr::firstTopic(string topic)
    {
        std::lock_guard<std::mutex> lock(first_lock);
        list<FirstTopic> lst;
        auto it=mapfirst.find(topic);
        if (it != mapfirst.end())
        {
            return it->second;
        }
        return lst;
    }
    void PubMgr::firstTopicCache(string topic, char msg[], int len, int64_t id)
    {
        std::lock_guard<std::mutex> lock(first_lock);
        auto it = mapfirst.find(topic);
        if (it != mapfirst.end())
        {
            FirstTopic f{ msg,len ,id};
            it->second.push_back(f);
        }
        else
        {
            list<FirstTopic> lst;
            FirstTopic f{ msg,len ,id};
            lst.push_back(f);
            mapfirst[topic] = lst;
        }
    }


    FirstTopic PubMgr::getTopicCache(string topic)
    {
        std::lock_guard<std::mutex> lock(first_lock);
        auto it = mapfirst.find(topic);
        FirstTopic f{nullptr,0};
        if (it != mapfirst.end())
        {
            if (!it->second.empty())
            {
               f = it->second.front();
              
               it->second.pop_front();
            }
            else
            {
                mapfirst.erase(topic);
            }
        }
        return f;
    }
    void	PubMgr::checkErrRecord()
    {

        thread check([&]()
            {
                int num = std::thread::hardware_concurrency();
                for (int i = 0;i < num; i++)
                {
                    // ??????
                    ErrorQueue tmp;
                    lstError.push_back(new ErrorQueue);

                }
                // ????????????????????????????????????????
                while (true)
                {
                    Records p;
                    errorRecords->wait_dequeue(p);
                    auto cur = recordsProcess(p);
                    if (cur->IsComlepte)
                    {
                        _semaphore.wait();//????????????????????????????????????
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
                        //??????????????
                        NngDataNative nng;
                        int64_t msgid = 0;
                        int dlen = 0;
                        auto buf = Util::Convert(p.Topic, kv.second, strlen(kv.second), '0', msgid, dlen);
                        int len = 0;
                        auto ret = nng.send(kv.first, buf, len);
                      
                        if (ret == nullptr || len == 0)
                        {
                            //????????????????
                            auto addr = SubTable::GetInstance()->getAddressLst(p.Topic);
                          
                              //??????????????????????????????????????
                              //??????????????????????????????????????
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
                                //????????????????????????????
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
                                //??????????????????????????????
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
                                    //????????????????,??????????????????2??????????????????????????????
                                     //????????????????2??
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
                                            //????????????
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
                                         // Console.WriteLine("??????????????????????,{0}>>{1}>>", kv.Key, addr.SubAddresses.Count);
                                        continue;
                                    }
                                    cur = err.AllAddress;//??????
                                    err.NumAll++;
                                }

                                //
                                for (auto right : cur)
                                {
                                    int dlen = 0;
                                    auto buf = Util::Convert(p.Topic, kv.second,strlen(kv.second), '0', msgid, dlen);
                                    int len = 0;
                                    auto ret = nng.send(right, buf, len);

                                    if (ret != nullptr && len > 0)
                                    {
                                        //??????????????????
                                        err.ErrorAddress.push_back(err.Address);
                                        err.Address = right;
                                        dicNodeGuid[kv.first] = err.NodeFlage;//????????????????????????
                                       /* Console.WriteLine("??????????????" + right);
                                        if (LocalNode.IsMsgReturn)
                                        {
                                            MsgTopicCount.Instance.Add(new PubRecords(){ SucessNum = 1, MsgId = p.MsgId });
                                        }*/
                                        break;
                                    }
                                    else
                                    {
                                        //????????????????
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

    uint64_t PubMgr::send(string topic, char msg[],int len)
    {
     
        int lenm = 0;
    
        //????????????????????????
        map<string, char*> dic ;

        //????????????????????????
        auto lst = SubTable::GetInstance()->getAddress(topic);
        int64_t id = msgid.fetch_add(1);
       
        if (!lst.empty())
        {

            NngDataNative nng;
            PubRecords records{ 0,0,0 };
            for (auto p : lst)
            {
                int msglen;
               
                auto buf = Util::Convert(topic, msg,len, '0', id, msglen);
                auto ret = nng.send(p, buf, msglen);
                if (ret == nullptr || msglen == 0)
                {
                    //????????????????
                  
                    dic[p] = msg;
                    records.FaildNum++;
                }
                else
                {
                    records.SucessNum++;
                }
                delete buf;
            }
            if (!dic.empty())
            {
                //??????????????????????????
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

            //????????????????
            auto lstFirst = firstTopic(topic);
            if (!lstFirst.empty())
            {
                //????????????;
                char* tmp = new char[len];
                std::memcpy(tmp, msg, len);
                firstTopicCache(topic, tmp, len,id);
                return id;
            }
            auto lstPub = PubTable::GetInstance()->getAddress(topic);//??????????????????
            if (!lstPub.empty())
            {
                //????????????????????????????,????????????????????????
                string find;
                for (auto tmp : lstPub)
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
            SubMgr::GetInstance()->openChanel();//????????????????????????
            //                             //????????????????
            auto lstLocal = topicBroadcast->pgmPub(topic);

            //????????????????????
            for (auto p : lstLocal)
            {
                
               // std::cout << "??????????????????:" + p << std::endl;
                PubTable::GetInstance()->add(topic, p);
            }
         
              _sub.wait();
             
               char* tmp = new char[len];
               std::memcpy(tmp, msg, len);
               firstTopicCache(topic, tmp, len,id);
               thread th([=]()
                {
                    
                    //????20????????50ms,1????????????????????????????????????????????????????
                    //??????????????????????????????????????????
                     NngDataNative nng;
                    for (int i = 0; i < 20; i++)
                    {
                       //????100ms??????????
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        //??????????????????;
                        PubRecords records{ 0,0,0 };
                        auto lst = SubTable::GetInstance()->getAddress(topic);
                        if (!lst.empty())
                        {
                            for (auto p : lst)
                            {
                                int msglen = 0;
                               
                                while (true)
                                {
                                    auto item = getTopicCache(topic);
                                  
                                    if (item.msg != nullptr)
                                    {
                                        auto buf = Util::Convert(topic, item.msg, item.len, '0', item.msgId, msglen);
                                      
                                        auto ret = nng.send(p, buf, msglen);
                                        if (ret == nullptr)
                                        {
                                            records.FaildNum++;
                                        }
                                        else
                                        {
                                            records.SucessNum++;
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                    delete item.msg;
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
