
#include "SubTable.h"
#include <iostream>

namespace libminimsgbus
{
	SubTable::SubTable()
	{
	}
   
    list<string> SubTable::getAddress(string topic)
    {
        list<string> lst;
        //SubAddressLst lstTMp;
        auto lstTMp = topicPub.find(topic);
        if (lstTMp != topicPub.end())
        {
            int num = lstTMp->second.SubAddresses.size();
            for (auto tmp : lstTMp->second.SubAddresses)
            {
                lst.push_back(tmp.Address);
            }
        }
        return lst;
    }

	bool SubTable::add(string topic, string address, string node)
    {
        
        std::cout << "接收到注册信息，主题:" << topic << "  地址:" << address<< " 标识 :" << node << std::endl;
        auto lst = topicPub.find(topic);
        if (lst != topicPub.end())
        {
            for (auto p : (lst->second.LstAddress))
            {
                if (p == address)
                {
                    return false;
                }
            }
            //
            lst->second.LstAddress.push_back(address);
            //同一节点加入
            for (auto p : lst->second.SubAddresses)
            {
                if (p.NodeFlage == node)
                {
                    p.AllAddress.push_back(address);
                    return true;
                }
                //  // 没有此节点
                SubAddress tmp;
                tmp.Address = address;
                tmp.NodeFlage = node;
                tmp.AllAddress.push_back(address);
                lst->second.SubAddresses.push_back(tmp);

            }

        }
        else
        {
            SubAddressLst lstNode;
            lstNode.LstAddress.push_back(address);
            SubAddress tmp;
            tmp.Address = address;
            tmp.NodeFlage = node;
            tmp.AllAddress.push_back(address);
            lstNode.SubAddresses.push_back(tmp);
            topicPub[topic] = lstNode;
        }
        return true;
    }
    void SubTable::remove(string topic, string node)
    {
        auto lst = topicPub.find(topic);
        if (lst != topicPub.end())
        {
           
            list<SubAddress>::iterator it = lst->second.SubAddresses.begin();
            for (; it != lst->second.SubAddresses.end();)
            {
                if (it->NodeFlage == node)
                {
                    lst->second.SubAddresses.erase(it++);
                    for (auto addr : it->AllAddress)
                    {
                         lst->second.LstAddress.remove(addr);
                    }
                    break;
                }

            }
            //for (auto p : lst->second.SubAddresses)
            //{
            //    if (p.NodeFlage == node)
            //    {
            //        lst->second.SubAddresses.remove(p);
            //        for (auto addr : p.AllAddress)
            //        {
            //           // lst->second.LstAddress.remove(addr);
            //        }
            //        break;
            //    }

            //}

        }
    }
    SubAddressLst SubTable::getAddressLst(string topic)
    {
        SubAddressLst lstTMp;

        auto lst = topicPub.find(topic);
        if (lst != topicPub.end())
        {
            lstTMp = lst->second;
        }
        return lstTMp;
    }
}
