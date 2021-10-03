#pragma once
#include <atomic>
#include <string>
#include<list>
#include<map>
#include <mutex>
#include "IMiniMsgBus.h"
using namespace std;
namespace libminimsgbus
{
	/// <summary>
	/// 关联数据
	/// </summary>
	class ObjSubMgr
	{
		static std::atomic<long> objPoint;
		static std::map <string, list<IMiniMsgBus*>> mapsub;
		static std::mutex mtx;
	public:

		/// <summary>
		/// 接收订阅数据
		/// </summary>
		/// <param name="topic">主题</param>
		/// <param name="bytes">数据</param>
		/// <param name="len">数据长度</param>
		static void receiveTopic(string topic, char* bytes, int len);

		/// <summary>
		/// 获取ID
		/// </summary>
		/// <returns></returns>
		static long getSubscriber();

		/// <summary>
		/// 加入
		/// </summary>
		/// <param name="topic"></param>
		/// <param name="bus"></param>
		static void holdTopic(string topic, IMiniMsgBus* bus);
	};
}

