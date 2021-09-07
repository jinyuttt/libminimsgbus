
#include "BridgeCore.h"
namespace libminimsgbus
{
	void BridgeCore::start()
	{
		for (auto addr : pubAddress)
		{
			NngDataNative *nng=new NngDataNative();
			nng->topicurl = addr;
			lstnngpub.push_back(nng);
		}
		for (auto addr : recAddress)
		{
			 NngDataNative *nng= new NngDataNative();
			 nng->pull(addr);
			 lstnng.push_back(nng);
			 thread  pull([&]()
				{
				  auto msg=	nng->getMsg();
				  list<NngDataNative*>::iterator pl;
				  for (pl = lstnngpub.begin(); pl != lstnngpub.end(); ++pl)
				  {
					  (*pl)->publish(msg.head, msg.bufdata, msg.size);
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