#include "pch.h"
#include "PubTable.h"
namespace libminimsgbus
{ 
	PubTable::PubTable()
	{
	}

    

	bool PubTable::add(string topic, string address)
    {
        auto v = topicPub.find(topic);
        if (v != topicPub.end())
        {
            for (auto lst : v->second)
            {
                if (lst == address)
                {
                    return false;
                }
            }
            v->second.push_back(address);
        }

        else
        {
            list<string> lst;
            lst.push_back(address);
            topicPub[topic] = lst;
        }
        return true;
    }

    list<string> PubTable::getAddress(string topic)
    {

        list<string> lst;
        auto v = topicPub.find(topic);
        if (v != topicPub.end())
        {
            lst = v->second;
        }
        return lst;
    }

    map<string, list<string>> PubTable::getPairs()
    {
        map<string, list<string>> tmp(topicPub);
        return tmp;
    }

}
