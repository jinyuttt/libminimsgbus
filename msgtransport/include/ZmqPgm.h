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
		string MultAddress = "239.192.1.1:4113";

		FBC_API ZmqPgm();
		FBC_API 	~ZmqPgm();
		FBC_API void subscribe(string topic);

		FBC_API void publish(string topic, char buf[]);

	};
}

