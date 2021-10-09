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
		/// ��������
		/// </summary>
		/// <param name="topic"></param>
		/// <param name="msgtopic"></param>
		void addSubscribe(string topic, msgtopic* msgtopic);

		/// <summary>
		/// ȡ������
		/// </summary>
		/// <param name="topic"></param>
		void unsubscribe(string topic);

		/// <summary>
		/// �Ƴ�����
		/// </summary>
		/// <param name="msgtopic"></param>
		void remove(msgtopic* msgtopic);
	};
}