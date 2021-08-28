#pragma once
#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>
#include <nngpp/protocol/rep0.h>
#include <nngpp/option.h>
#include <string>
#include <thread>
#include "BlockingConcurrentQueue.h"
#include "defMsg.h"

using namespace std;
using namespace moodycamel;

#ifdef _MSC_VER
#ifdef FBC_STATIC
#define FBC_API
#elif defined FBC_EXPORT
#define FBC_API __declspec(dllexport)
#else
#define FBC_API __declspec(dllimport)
#endif

#endif
namespace  msgtransport {



	class NngDataNative
	{
	private:
		nng::socket lissock;
		 char* repMsg = nullptr;
		bool isStop = false;
		BlockingConcurrentQueue<MsgBuffer> queue;

		/// <summary>
		///线程接收
		/// </summary>
		void recviceThread();

		/// <summary>
		/// 回复数据
		/// </summary>
		/// <returns></returns>
		 char* repMessage();

		 int inttoIp(uint32_t uiIp, string& strOut);
		 int iptoInt(char* ipStr, uint32_t& ipInt);
		 void ipv6_to_str(string& addr_str, uint32_t ipv6_addr[]);
		 void ipv6_to_str(string& addr_str, uint8_t ipv6_addr[]);
	public:

		/// <summary>
		/// 发送数据
		/// </summary>
		/// <param name="address">地址</param>
		/// <param name="bytes">数据</param>
		/// <param name="len">数据长度</param>
		/// <returns></returns>
		FBC_API  char* send(string address, char bytes[], int* len);

		/// <summary>
		/// 接收数据
		/// </summary>
		/// <returns></returns>
		FBC_API  MsgBuffer getData();

		/// <summary>
		/// 开启接收数据
		/// </summary>
		/// <param name="address">本地地址</param>
		/// <returns></returns>
		FBC_API  string  receive(string address);

		/// <summary>
		/// 关闭接收
		/// </summary>
		FBC_API void close();
	};
}

