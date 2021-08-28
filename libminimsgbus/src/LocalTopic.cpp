#include "pch.h"
#include "LocalTopic.h"
namespace libminimsgbus
{
	map<string, msgtopic*> LocalTopic::localSub;
	void LocalTopic::AddLocal(string topic, msgtopic* ov)
	{
		localSub[topic] = ov;
	}
	msgtopic* LocalTopic::GetLocal(string topic)
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