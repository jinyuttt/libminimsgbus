#pragma once
#include <thread>
#include<iterator>
#include <mutex>
#include<list>
#include "TopicBroadcast.h"
#include "SubTable.h"
#include "Util.h"
#include "PubTable.h"
#include "ErrorQueue.h"
#include "BlockingConcurrentQueue.h"
#include "bussemaphore.h"
#include "MsgLocalNode.h"
#include "MsgStruct.h"
using namespace std;
using namespace msgtransport;
using namespace moodycamel;
namespace libminimsgbus
{
    class SubMgr;
    class PubMgr
    {
    private:  //Ĭ�Ϲ���Ϳ������춼��Ҫ��private
        PubMgr();
        PubMgr(const PubMgr& other);

        void init();

        list<FirstTopic> firstTopic(string topic);
        void firstTopicCache(string topic,char msg[],int len,int64_t id);
        FirstTopic getTopicCache(string topic);
    public:
        static PubMgr* GetInstance() {
            static PubMgr instance;
            return &instance;
        }
        BlockingConcurrentQueue<Records>* errorRecords = nullptr;
        const int waitTime = 1000 * 60;//1����
        list<ErrorQueue*> lstError;
        bussemaphore _semaphore;
        bussemaphore _sub;
        TopicBroadcast *topicBroadcast;
        map<string, string> dicNodeGuid;
        std::atomic<int64_t> msgid;
        std::mutex first_lock;//�����״η�������
        std::map<string, list<FirstTopic>> mapfirst;
        /// <summary>
        /// �̴߳����쳣����
        /// </summary>
        void checkErrRecord();


        ErrorQueue* recordsProcess(Records queue);


        /// <summary>
        /// �����쳣����
        /// </summary>
        /// <param name="queue"></param>
        void startThreadRecord(ErrorQueue& queue);


        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="msg"></param>
        uint64_t send(string topic, char msg[],int len);
    };
};