
#include "IpcMsgBus.h"
#include"ObjSubMgr.h"
namespace libminimsgbus
{
   
   
   
    uint64_t IpcMsgBus::publish(string topic, char* bytes, int len)
    {
        if (topic.empty())
        {
            topic = defaultTopic;
        }
        topicIpc.ipcSend(topic, bytes,len);
        return 0;
    }

    void IpcMsgBus::subscribe(string topic)
    {
        if (topic.empty())
        {
            topic = defaultTopic;
        }
        if (isInit)
        {
          
            topicIpc.callback = this->callback;
            topicIpc.recmsgtopic = this->revmsg;
            objPoint = ObjSubMgr::getSubscriber();
            isInit = false;
            topicIpc.ipcRecv();
           
        }
       /* ObjSubMgr::holdTopic(topic, this);*/
    }

    void IpcMsgBus::unsubscribe(string topic)
    {
        ObjSubMgr::remove(topic, this);
    }

}