#include "pch.h"
#include "ObjSubMgr.h"
#include "IpcMsgBus.h"
namespace libminimsgbus {
    atomic<long> ObjSubMgr::objPoint = 0;
    map <string, list<IpcMsgBus*>> ObjSubMgr::mapsub;
    std::mutex ObjSubMgr::mtx;
    void ObjSubMgr::receiveTopic(string topic, char* bytes, int len)
    {
       auto findObj= mapsub.find(topic);
       if (findObj != mapsub.end())
       {
           auto &items= findObj->second;
           for (auto item : items)
           {
               item->revmsg(topic, bytes, len);
           }
       }
    }
    void ObjSubMgr::holdTopic(string topic, IpcMsgBus* bus)
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
            list<IpcMsgBus*> lst;
            lst.push_back(bus);
            mapsub[topic] = lst;
            
        }
    }
    long ObjSubMgr::getSubscriber()
    {
        return objPoint.fetch_add(1);
    }
   
}
