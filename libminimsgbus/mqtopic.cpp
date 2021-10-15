#include "mqtopic.h"

void libminimsgbus::mqtopic::publish(string topic, char bytes[], int len)
{
	nng.push(address, bytes, len, topic);
}

void libminimsgbus::mqtopic::subscribe(string topic)
{
	nng.subscribe(topic);
	if (!ready)
	{
		ready = true;
		thread rc([&]() {
			 auto buf=nng.getMsg();
			 if (callback != nullptr)
			 {
				 callback(buf.head, buf.bufdata, buf.size);
			 }
			 if (recmsgtopic != nullptr)
			 {
				 recmsgtopic(buf.head, buf.bufdata, buf.size);
			 }
			});
		rc.detach();
	}
}
