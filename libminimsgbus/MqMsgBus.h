#pragma once
#include "IMiniMsgBus.h"
#include "mqtopic.h"
#include <thread>
#include "MqMsgNng.h"
#include <map>
namespace libminimsgbus
{
	class MqMsgBus : public IMiniMsgBus
	{
	
		volatile bool isInit = true;
		map<string, string> mapTopic;
	public:
		/// <summary>
	/// 接收回调
	/// </summary>
	/// <param name="topic"></param>
	/// <param name="data"></param>
	/// <param name="len"></param>
		void revmsgtopic(string topic, char* data, int len);

		
		virtual uint64_t publish(string topic, char* bytes, int len) override;

		virtual void subscribe(string topic) override;

		// 通过 IMiniMsgBus 继承
		virtual void unsubscribe(string topic) override;
	};
}
