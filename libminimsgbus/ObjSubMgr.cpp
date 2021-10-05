
#include "ObjSubMgr.h"
#include "IpcMsgBus.h"
namespace libminimsgbus {
    atomic<long> ObjSubMgr::objPoint ;
    map <string, list<IMiniMsgBus*>> ObjSubMgr::mapsub;
    std::mutex ObjSubMgr::mtx;
    void ObjSubMgr::receiveTopic(string topic, char* bytes, int len)
    {
       auto findObj= mapsub.find(topic);
       
       if (findObj != mapsub.end())
       {
           auto &items= findObj->second;
           for (auto item : items)
           {
               if (item->revmsg != nullptr)
               {
                   item->revmsg(topic, bytes, len);
               }
               else if (item->callback != nullptr)
               {
                   item->callback(topic, bytes, len);
               }
           }
       }
    }
    void ObjSubMgr::holdTopic(string topic, IMiniMsgBus* bus)
    {
        std::lock_guard<std::mutex> lck(mtx);
        auto findObj = mapsub.find(topic);
        if (findObj != mapsub.end())
        {
            auto lst = findObj->second;
            bool isFind = false;
            for (auto item : lst)
            {
                if (item == bus)
                {
                    return;
                }
            }
            lst.push_back(bus);
        }
        else
        {
            list<IMiniMsgBus*> lst;
            lst.push_back(bus);
            mapsub[topic] = lst;
            
        }
    }
    long ObjSubMgr::getSubscriber()
    {
        return objPoint.fetch_add(1);
    }
   
}
