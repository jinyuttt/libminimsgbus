#pragma once

#include<string>
#include<list>
#include"NngDataNative.h"
#include "MsgDef.h"
using namespace msgtransport;
using namespace std;
namespace libminimsgbus
{
	/// <summary>
	/// MQ中心节点
	/// </summary>
	class BridgeCore
	{
	private:
		list<NngDataNative*> lstnng;
		list<NngDataNative*> lstnngpub;
	public:

		/// <summary>
		/// 接收发布数据地址
		/// </summary>
		list<string> recAddress;

		/// <summary>
		/// 转发地址，接收订阅方的地址
		/// </summary>
		list<string> pubAddress;

		FBC_API void start();

		FBC_API void close();
	};
}

