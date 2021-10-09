
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
                    // 初始化
                    ErrorQueue tmp;
                    lstError.push_back(new ErrorQueue);

                }
                // 每个主题，循环处理每个主题发送失败的数据
                while (true)
                {
                    Records p;
                    errorRecords->wait_dequeue(p);
                    auto cur = recordsProcess(p);
                    if (cur->IsComlepte)
                    {
                        _semaphore.wait();//等待前面的主题处理完成，不能太多线程
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
                        //将数据再次发送
                        NngDataNative nng;
                        int64_t msgid = 0;
                        int dlen = 0;
                        auto buf = Util::Convert(p.Topic, kv.second, strlen(kv.second), '0', msgid, dlen);
                        int len = 0;
                        auto ret = nng.send(kv.first, buf, len);
                      
                        if (ret == nullptr || len == 0)
                        {
                            //获取本地订阅节点
                            auto addr = SubTable::GetInstance()->getAddressLst(p.Topic);
                          
                              //还是失败，判断是否是多网卡绑定订阅地址
                              //与当前节点使用的通信地址一致的地址信息
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
                                //没有找到，可能前面已经替换了
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
                                //返回判断非异常的地址，同一节点
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
                                    //已经没有正确地址,节点所有地址都发送2次后移除节点，任务订阅节点异常
                                     //这里依托数据发送2次
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
                                            //无用的数据了
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
                                         // Console.WriteLine("异常处理节点没有结果了,{0}>>{1}>>", kv.Key, addr.SubAddresses.Count);
                                        continue;
                                    }
                                    cur = err.AllAddress;//全尝试
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
                                        //收到回复该地址正常
                                        err.ErrorAddress.push_back(err.Address);
                                        err.Address = right;
                                        dicNodeGuid[kv.first] = err.NodeFlage;//将当前处理正常的地址保存
                                       /* Console.WriteLine("成功处理异常，" + right);
                                        if (LocalNode.IsMsgReturn)
                                        {
                                            MsgTopicCount.Instance.Add(new PubRecords(){ SucessNum = 1, MsgId = p.MsgId });
                                        }*/
                                        break;
                                    }
                                    else
                                    {
                                        //节点异常地址加入
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
    
        //记录发送异常的地址和数据
        map<string, char*> dic ;

        //从本地已经订阅的地址查找
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
                    //发布失败没有返回
                  
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
                //存入异常数据，等待再次处理
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

            //本地没有订阅节点
            auto lstFirst = firstTopic(topic);
            if (!lstFirst.empty())
            {
                //还在等待发送;
                char* tmp = new char[len];
                std::memcpy(tmp, msg, len);
                firstTopicCache(topic, tmp, len,id);
                return id;
            }
            auto lstPub = PubTable::GetInstance()->getAddress(topic);//从全局发布表中查询
            if (!lstPub.empty())
            {
                //本节点已经发布过地址就丢数据,说明没有节点订阅这个主题
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
            SubMgr::GetInstance()->openChanel();//初始化，启动数据接收订阅
            //                             //第一次本节点发布
            auto lstLocal = topicBroadcast->pgmPub(topic);

            //将新发布节点加入本地
            for (auto p : lstLocal)
            {
                
               // std::cout << "本节点加入发布地址:" + p << std::endl;
                PubTable::GetInstance()->add(topic, p);
            }
         
              _sub.wait();
             
               char* tmp = new char[len];
               std::memcpy(tmp, msg, len);
               firstTopicCache(topic, tmp, len,id);
               thread th([=]()
                {
                    
                    //等待20次，每次50ms,1秒了完成发布，否则数据丢失；因为地址通知考虑没有回复
                    //通知地址后不会有消息回复，会增加消息交互量
                     NngDataNative nng;
                    for (int i = 0; i < 20; i++)
                    {
                       //等待100ms取订阅地址
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        //再次检查是否有订阅;
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
