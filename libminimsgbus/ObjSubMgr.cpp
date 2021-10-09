
#include "ObjSubMgr.h"
#include "IpcMsgBus.h"
namespace libminimsgbus {
    atomic<long> ObjSubMgr::objPoint ;
    map <string, list<IMiniMsgBus*>> ObjSubMgr::mapsub;
   
    std::mutex ObjSubMgr::mtx;
    void ObjSubMgr::receiveTopic(string topic, char* bytes, int len)
    {
        std::lock_guard<std::mutex> lck(mtx);
       auto findObj= mapsub.find(topic);
       
       if (findObj != mapsub.end())
       {
           try
           {
               auto& items = findObj->second;
               for (auto item : items)
               {
                   //调用netmsgbus
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
           catch (std::exception e)
           {
               std::cout << e.what() << std::endl;
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

    void ObjSubMgr::remove(string topic, IMiniMsgBus* bus)
    {
        std::lock_guard<std::mutex> lck(mtx);
        auto findObj = mapsub.find(topic);
       
        if (findObj != mapsub.end())
        {
            auto lst = findObj->second;
            bool isFind = false;
            auto it = lst.begin();
            if (lst.empty())
            {
                mapsub.erase(topic);
                return;
            }
            for (it = lst.begin(); it != lst.end(); it++)
            {
                if (*it == bus)
                {
                   //不能释放对象，可能有其他主题使用
                    lst.erase(it++);
                    break;
                }
            }
        }
    }

    bool ObjSubMgr::empty(string topic)
    {
        auto findObj = mapsub.find(topic);

        if (findObj != mapsub.end()||findObj->second.empty())
        {
            return true;
        }
        return false;
    }

    list<string> ObjSubMgr::erase(IMiniMsgBus* bus)
    {
        //找到所有主题使用的bus
         std::lock_guard<std::mutex> lck(mtx);
         list<string> lstempty;
         auto  iter = mapsub.begin();
         while (iter != mapsub.end()) {
            auto item = iter->second.begin();
            while (item != iter->second.end())
            {
                if (*item == bus)
                {
                    bus->callback = nullptr;
                    bus->revmsg = nullptr;
                    iter->second.erase(item++);
                    break;
                }
                item++;
            }
            if (iter->second.empty())
            {
                lstempty.push_back(iter->first);
            }
            iter++;
        }
         return lstempty;
    }

    long ObjSubMgr::getSubscriber()
    {
        return objPoint.fetch_add(1);
    }
   
}
