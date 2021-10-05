
#include "ObjSubMgr.h"
#include "IpcMsgBus.h"
namespace libminimsgbus {
    atomic<long> ObjSubMgr::objPoint ;
    map <string, list<IMiniMsgBus*>> ObjSubMgr::mapsub;
    map <string, int> ObjSubMgr::mapsubNum;
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

    void ObjSubMgr::holdTopic(string topic, IMiniMsgBus* bus, int busType)
    {
        std::lock_guard<std::mutex> lck(mtx);
        auto findObj = mapsub.find(topic);
        auto it = mapsubNum.find(topic);
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
            if(busType==1)
             it->second++;
        }
        else
        {
            list<IMiniMsgBus*> lst;
            lst.push_back(bus);
            mapsub[topic] = lst;
            if (busType == 1)
              mapsubNum[topic] = 1;
        }
    }

    void ObjSubMgr::remove(string topic, IMiniMsgBus* bus, int busType)
    {
        std::lock_guard<std::mutex> lck(mtx);
        auto findObj = mapsub.find(topic);
        auto itnum = mapsubNum.find(topic);
        if (findObj != mapsub.end())
        {
            auto lst = findObj->second;
            bool isFind = false;
            auto it = lst.begin();
            if (lst.empty())
            {
                mapsub.erase(topic);
                mapsubNum.erase(topic);
                return;
            }
            for (; it != lst.end(); ++it)
            {
                if (*it == bus)
                {
                    delete *it;
                    lst.erase(it++);
                }
            }
            if (busType == 1)
            {
                if (itnum != mapsubNum.end())
                {
                    itnum->second--;
                }
            }
        }
    }

    bool ObjSubMgr::empty(string topic)
    {
        auto itnum = mapsubNum.find(topic);
        if (itnum == mapsubNum.end() || itnum->second < 1)
        {
            return true;
        }
        return false;
    }

    long ObjSubMgr::getSubscriber()
    {
        return objPoint.fetch_add(1);
    }
   
}
