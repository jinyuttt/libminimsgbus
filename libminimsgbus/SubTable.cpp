
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
        
        std::cout << "���յ�ע����Ϣ������:" << topic << "  ��ַ:" << address<< " ��ʶ :" << node << std::endl;
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
            //ͬһ�ڵ����
            for (auto p : lst->second.SubAddresses)
            {
                if (p.NodeFlage == node)
                {
                    p.AllAddress.push_back(address);
                    return true;
                }
                //  // û�д˽ڵ�
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
