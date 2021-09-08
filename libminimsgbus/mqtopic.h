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
	/// �����ĵķ�������
	/// </summary>
	class mqtopic
	{
		NngDataNative nng;
		std::atomic<bool>  ready{ false };
	public:

		/// <summary>
		/// ���Ļص�
		/// </summary>
		ReceiveTopic  recmsgtopic = nullptr;

		/// <summary>
		/// ���Ļص�
		/// </summary>
		std::function<void(std::string, char*, int)> callback;

		/// <summary>
		/// ���ĵ�ַ�򷢲���ַ
		/// </summary>
		string address;

	   /// <summary>
	   /// ��������
	   /// </summary>
	   /// <param name="topic"></param>
	   /// <param name="bytes"></param>
	   /// <param name="len"></param>
	   void	publish(string topic, char bytes[], int len);

	   /// <summary>
	   /// ��������
	   /// </summary>
	   /// <param name="topic"></param>
	   void subscribe(string topic);
	};
}

