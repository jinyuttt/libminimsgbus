#pragma once
#include <thread>
#include<iterator>
#include "MsgDef.h"
#include "TopicBroadcast.h"
#include "SubTable.h"
#include "Util.h"
#include "PubTable.h"
#include "ErrorQueue.h"
#include "BlockingConcurrentQueue.h"
#include "bussemaphore.h"
#include "MsgLocalNode.h"
using namespace std;
using namespace msgtransport;
using namespace moodycamel;
namespace libminimsgbus
{
    class SubMgr;
    class PubMgr
    {
    private:  //默认构造和拷贝构造都需要是private
        PubMgr();
        PubMgr(const PubMgr& other);

        void init();
    public:
        static PubMgr* GetInstance() {
            static PubMgr instance;
            return &instance;
        }
        BlockingConcurrentQueue<Records>* errorRecords = nullptr;
        const int waitTime = 1000 * 60;//1分钟
        list<ErrorQueue*> lstError;
        bussemaphore _semaphore;
        bussemaphore _sub;
        TopicBroadcast *topicBroadcast;
        map<string, string> dicNodeGuid;
        //uint64_t msgid = 0;
        std::atomic<int64_t> msgid;

        /// <summary>
        /// 线程处理异常数据
        /// </summary>
        void checkErrRecord();


        ErrorQueue* recordsProcess(Records queue);


        /// <summary>
        /// 处理异常数据
        /// </summary>
        /// <param name="queue"></param>
        void startThreadRecord(ErrorQueue& queue);


        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="msg"></param>
        uint64_t send(string topic, char msg[]);
    };
};