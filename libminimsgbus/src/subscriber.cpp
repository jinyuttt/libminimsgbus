#include "pch.h"
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
};