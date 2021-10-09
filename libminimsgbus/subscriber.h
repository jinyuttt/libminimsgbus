#pragma once
#include <map>
#include "SubMgr.h"
#include "msgtopic.h"
using namespace std;
namespace libminimsgbus {
	
	class subscriber
	{
	public:
		subscriber();
		~subscriber();

		/// <summary>
		/// 订阅主题
		/// </summary>
		/// <param name="topic"></param>
		/// <param name="msgtopic"></param>
		void addSubscribe(string topic, msgtopic* msgtopic);

		/// <summary>
		/// 取消订阅
		/// </summary>
		/// <param name="topic"></param>
		void unsubscribe(string topic);

		/// <summary>
		/// 移除对象
		/// </summary>
		/// <param name="msgtopic"></param>
		void remove(msgtopic* msgtopic);
	};
}