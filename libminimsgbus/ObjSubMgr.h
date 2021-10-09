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
	/// 关联网络订阅
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

		/// <summary>
		/// 移除订阅
		/// </summary>
		/// <param name="topic"></param>
		/// <param name="bus"></param>
		static void remove(string topic, IMiniMsgBus* bus);

		/// <summary>
		/// 判断网络订阅主题全部注销
		/// </summary>
		/// <param name="topic"></param>
		/// <returns></returns>
		static bool empty(string topic);

		/// <summary>
		/// 删除对象
		/// </summary>
		/// <param name="bus"></param>
		static list<string> erase(IMiniMsgBus* bus);
		
	};
}

