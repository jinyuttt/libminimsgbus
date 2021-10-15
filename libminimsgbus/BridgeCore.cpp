
#include "BridgeCore.h"
#include <Util.h>
namespace libminimsgbus
{
	void BridgeCore::start()
	{
		for (auto addr : pubAddress)
		{
			NngDataNative *nng=new NngDataNative();
			nng->topicurl = addr;
			lstnngpub.push_back(nng);
			nng->publish("JY", "1", 1);
		}
		for (auto addr : recAddress)
		{
			 NngDataNative *nng= new NngDataNative();
			 nng->pull(addr);
			 lstnng.push_back(nng);
			 std::this_thread::sleep_for(std::chrono::milliseconds(100));
			    thread  pull([=]()
				{
					 while (true)
					 {
						 auto data = nng->getMsg();
						 //auto msg = Util::msgToTopic(data.bufdata, data.size);
						 list<NngDataNative*>::iterator pl;
						 for (pl = lstnngpub.begin(); pl != lstnngpub.end(); ++pl)
						 {
							 //(*pl)->publish(msg.Topic, msg.Msg, msg.msglen);
							 (*pl)->publish("", data.bufdata, data.size);
						 }
						// delete data.bufdata;
					 }
				 

				});
			 pull.detach();
		}
		
	    	
	}
	void BridgeCore::close()
	{
		list<NngDataNative*>::iterator pl;
		for (pl = lstnngpub.begin(); pl != lstnngpub.end(); ++pl)
		{
			(*pl)->close();
		}
		for (pl = lstnng.begin(); pl != lstnng.end(); ++pl)
		{
			(*pl)->close();
		}
	}
}