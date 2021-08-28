#pragma once
#include <atomic>
#include <string>
#include<list>
#include<map>
#include <mutex>
using namespace std;
namespace libminimsgbus
{
	class IpcMsgBus;
	class ObjSubMgr
	{
		static std::atomic<long> objPoint;
		static std::map <string, list<IpcMsgBus*>> mapsub;
		static std::mutex mtx;
	public:
		static void receiveTopic(string topic, char* bytes, int len);
		static long getSubscriber();
		//static void hold(long id, IpcMsgBus* bus);
		static void holdTopic(string topic, IpcMsgBus* bus);
	};
}

