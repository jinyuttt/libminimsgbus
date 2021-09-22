#pragma once
#include "IMiniMsgBus.h"
#include "mqtopic.h"
namespace libminimsgbus
{
	class MqMsgBus : public IMiniMsgBus
	{
	
		mqtopic mq;
		volatile bool isInit = true;
		/// <summary>
		/// 接收回调
		/// </summary>
		/// <param name="topic"></param>
		/// <param name="data"></param>
		/// <param name="len"></param>
		void revmsgtopic(string topic, char* data, int len);
	public:
		
		virtual uint64_t publish(string topic, char bytes[], int len) override;

		virtual void subscribe(string topic) override;
	};
}
