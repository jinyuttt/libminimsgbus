#include "MqMsgNng.h"
#include "MqMsgBus.h"
#include <Util.h>
namespace libminimsgbus
{
	void MqMsgNng::publish(string url, string topic, char* bytes, int len)
	{
		int dlen = 0;
		auto data = Util::convertMsg(topic,bytes,len,dlen);
		nng->push(url, data, dlen);
	}
	void MqMsgNng::subscribe(string url, string topic)
	{
		nng->topicurl = url;
		nng->subscribe(topic);
	}
	void MqMsgNng::recmsg()
	{
		thread rec([&] {
			while (true)
			{
				auto data = nng->getMsg();
				auto msg = Util::msgToTopic(data.bufdata,data.size);
				auto item = lstmq.begin();
				for (; item != lstmq.end();)
				{
					//(*item)->revmsgtopic(data.head, data.bufdata, data.size);
					(*item)->revmsgtopic(msg.Topic, msg.Msg, msg.msglen);
					item++;
				}
				//delete data.bufdata;
				delete msg.Msg;
			}
			});
		rec.detach();
	}

	void MqMsgNng::add(MqMsgBus* bus)
	{

		lstmq.push_back(bus);
	}
	MqMsgNng::MqMsgNng()
	{
		nng = new NngDataNative();
		recmsg();
	}
	MqMsgNng::~MqMsgNng()
	{
	}
	
}