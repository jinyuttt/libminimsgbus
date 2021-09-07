#pragma once
#include <string>
#include<functional>
#include"MsgDef.h"
#include <NngDataNative.h>
#include"defMsg.h"
using namespace std;
using namespace msgtransport;

namespace libminimsgbus
{
	/// <summary>
	/// 有中心的发布订阅
	/// </summary>
	class mqtopic
	{
		NngDataNative nng;
		std::atomic<bool>  ready{ false };
	public:
		ReceiveTopic  recmsgtopic = nullptr;
		string address;
		std::function<void(std::string, char*, int)> callback;
	   void	publish(string topic, char bytes[], int len);
	   void subscribe(string topic);
	};
}

