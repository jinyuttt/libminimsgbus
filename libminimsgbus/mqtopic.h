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

		/// <summary>
		/// 订阅回调
		/// </summary>
		ReceiveTopic  recmsgtopic = nullptr;

		/// <summary>
		/// 订阅回调
		/// </summary>
		std::function<void(std::string, char*, int)> callback;

		/// <summary>
		/// 订阅地址或发布地址
		/// </summary>
		string address;

	   /// <summary>
	   /// 发布数据
	   /// </summary>
	   /// <param name="topic"></param>
	   /// <param name="bytes"></param>
	   /// <param name="len"></param>
	   void	publish(string topic, char bytes[], int len);

	   /// <summary>
	   /// 订阅主题
	   /// </summary>
	   /// <param name="topic"></param>
	   void subscribe(string topic);
	};
}

