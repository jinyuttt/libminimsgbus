#pragma once
#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>
#include <nngpp/protocol/rep0.h>
#include <nngpp/protocol/push0.h>
#include <nngpp/protocol/pull0.h>
#include <nngpp/protocol/sub0.h>
#include <nngpp/protocol/pub0.h>
#include <nngpp/option.h>
#include <string>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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
#else
#define FBC_API
#endif
namespace  msgtransport {



	class NngDataNative
	{
	private:
		nng::socket lissock;
		nng::socket pullsock ;
		 char* repMsg = nullptr;
		bool isStop = false;

		/// <summary>
		/// req数据
		/// </summary>
		BlockingConcurrentQueue<MsgBuffer> queue;

		/// <summary>
		/// pull,sub数据
		/// </summary>
		BlockingConcurrentQueue<MsgBody> queuemsg;
		std::atomic<bool>  ready = std::atomic_bool(false); ;
		std::atomic<bool>  subread;
		/// <summary>
		///线程接收
		/// </summary>
		void recviceThread();

		/// <summary>
		/// 线程接收数据
		/// </summary>
		void recviceMsg();
		/// <summary>
		/// 回复数据
		/// </summary>
		/// <returns></returns>
		 char* repMessage();
		
		 /// <summary>
		 /// 数字转IP
		 /// </summary>
		 /// <param name="uiIp"></param>
		 /// <param name="strOut"></param>
		 /// <returns></returns>
		 int inttoIp(uint32_t uiIp, string& strOut);

		 /// <summary>
		 /// IP转换数字
		 /// </summary>
		 /// <param name="ipStr"></param>
		 /// <param name="ipInt"></param>
		 /// <returns></returns>
		 int iptoInt(char* ipStr, uint32_t& ipInt);

		 /// <summary>
		 /// IP6转字符串
		 /// </summary>
		 /// <param name="addr_str"></param>
		 /// <param name="ipv6_addr"></param>
		 void ipv6_to_str(string& addr_str, uint32_t ipv6_addr[]);
		 void ipv6_to_str(string& addr_str, uint8_t ipv6_addr[]);
	public:

		/// <summary>
		/// 订阅发布使用地址
		/// </summary>
		string topicurl;
		NngDataNative();
		~NngDataNative();
		/// <summary>
		/// 发送数据
		/// </summary>
		/// <param name="address">地址</param>
		/// <param name="bytes">数据</param>
		/// <param name="len">数据长度</param>
		/// <returns></returns>
		FBC_API  char* send(string address, char bytes[], int* len);

		/// <summary>
		/// 开启接收数据
		/// </summary>
		/// <param name="address">本地地址</param>
		/// <returns></returns>
		FBC_API  string  receive(string address);

		/// <summary>
		/// 接收数据
		/// </summary>
		/// <returns></returns>
		FBC_API  MsgBuffer getData();

		

		/// <summary>
		/// 抽取数据
		/// </summary>
		/// <param name="address">接收地址</param>
		/// <returns></returns>
		FBC_API  string  pull(string address);

		/// <summary>
		/// 推送数据
		/// </summary>
		/// <param name="address">地址</param>
		/// <param name="head">头标识</param>
		/// <param name="bytes"></param>
		/// <param name="len"></param>
		FBC_API  void  push(string address, char bytes[], int* len, string head="");

		/// <summary>
		/// 获取外部数据
		/// </summary>
		/// <returns></returns>
		FBC_API  MsgBody getMsg();

		/// <summary>
		/// 发布数据
		/// </summary>
		/// <param name="topic">主题</param>
		/// <param name="data">数据</param>
		/// <param name="len">长度</param>
		FBC_API  void  publish(string topic, char* data, int len);

		/// <summary>
		/// 订阅
		/// </summary>
		/// <param name="topic"></param>
		FBC_API  void  subscribe(string topic);

		/// <summary>
		/// 关闭接收
		/// </summary>
		FBC_API void close();
	};
}

