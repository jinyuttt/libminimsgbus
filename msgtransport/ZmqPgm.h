#pragma once
#include <string>
#include <list>
#include <mutex>
#include <iostream>
#include <functional>
#include "zmq.hpp"
#include "defMsg.h"
using namespace std;
namespace msgtransport {
	
	/// <summary>
	/// 使用zmq中的pgm
	/// </summary>
	class ZmqPgm
	{
	private:
		zmq::context_t ctx;
		zmq::socket_t subSocket;
		zmq::socket_t pubSocket;
		bool isStart = false;
		bool isBind = true;
		bool isSubBind = true;
	    
		mutex sigsocket;
		void ReceiveData();
		void ThreadReceive();

	public:
		list<string> LocalAddres;
		std::function<void(std::string, char*, int)> callback;

		//组播地址
		string MultAddress = "239.192.1.1:4113";

		FBC_API ZmqPgm();
		FBC_API 	~ZmqPgm();

		/// <summary>
		/// 订阅主题
		/// </summary>
		/// <param name="topic"></param>
		/// <returns></returns>
		FBC_API void subscribe(string topic);

		/// <summary>
		///发布数据
		/// </summary>
		/// <param name="topic"></param>
		/// <param name="buf"></param>
		/// <returns></returns>
		FBC_API void publish(string topic, char buf[]);

	};
}

