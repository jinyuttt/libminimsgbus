
#include "LocalTopic.h"
namespace libminimsgbus
{
	map<string, msgtopic*> LocalTopic::localSub;
	void LocalTopic::addLocal(string topic, msgtopic* ov)
	{
		localSub[topic] = ov;
	}
	msgtopic* LocalTopic::getLocal(string topic)
	{
		msgtopic* ov = nullptr;
		auto  cur = localSub.find(topic);
		if (cur != localSub.end())
		{
			ov = cur->second;
		}
		return ov;
	}
}