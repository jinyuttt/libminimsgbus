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
	/// ��������
	/// </summary>
	class ObjSubMgr
	{
		static std::atomic<long> objPoint;
		static std::map <string, list<IMiniMsgBus*>> mapsub;
		static std::mutex mtx;
	public:

		/// <summary>
		/// ���ն�������
		/// </summary>
		/// <param name="topic">����</param>
		/// <param name="bytes">����</param>
		/// <param name="len">���ݳ���</param>
		static void receiveTopic(string topic, char* bytes, int len);

		/// <summary>
		/// ��ȡID
		/// </summary>
		/// <returns></returns>
		static long getSubscriber();

		/// <summary>
		/// ����
		/// </summary>
		/// <param name="topic"></param>
		/// <param name="bus"></param>
		static void holdTopic(string topic, IMiniMsgBus* bus);
	};
}

