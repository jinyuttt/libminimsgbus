#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <regex>
#include <atomic>
#include <mutex>
#include <functional>
#include "libipc/ipc.h"
#include "defMsg.h"
using namespace std;
namespace msgtransport
{
	
	class IpcNative
	{
	
		bool isStart = false;
		constexpr static char const name_[] = "minimsgbus";
		constexpr static char const quit_[] = "q";
		constexpr static char const id_[] = "c";
		mutex sigRec;
		inline std::size_t calc_unique_id() {
			static ipc::shm::handle g_shm{ "__CHAT_ACC_STORAGE__", sizeof(std::atomic<std::size_t>) };
			return static_cast<std::atomic<std::size_t>*>(g_shm.get())->fetch_add(1, std::memory_order_relaxed);
		}

		ipc::channel sender_{ name_, ipc::sender };
		ipc::channel receiver_{ name_, ipc::receiver };
		void ReceiveData();
		void ReceiveThread();
	public:
		ReceiveTopic  recmsgtopic=nullptr;
		std::function<void(std::string, char*, int)> callback;
		
		FBC_API void ipcRecv();
		FBC_API void ipcSend(string topic, char buf[],int len);
	};
}

