
#include "subscriber.h"
namespace libminimsgbus {
	subscriber::subscriber()
	{
	}

	subscriber::~subscriber()
	{
	}

	void subscriber::addSubscribe(string topic, msgtopic* msgtopic)
	{
		SubMgr::GetInstance()->sendSub(topic, msgtopic);
	}
	void subscriber::unsubscribe(string topic)
	{
		SubMgr::GetInstance()->sendUnsub(topic);
	}
	void subscriber::remove(msgtopic* msgtopic)
	{

	}
};