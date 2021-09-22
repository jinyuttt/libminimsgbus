#pragma once

#include<string>
#include<list>
#include"NngDataNative.h"
using namespace msgtransport;
using namespace std;
namespace libminimsgbus
{
	/// <summary>
	/// MQ���Ľڵ�
	/// </summary>
	class BridgeCore
	{
	private:
		list<NngDataNative*> lstnng;
		list<NngDataNative*> lstnngpub;
	public:

		/// <summary>
		/// ���շ������ݵ�ַ
		/// </summary>
		list<string> recAddress;

		/// <summary>
		/// ת����ַ�����ն��ķ��ĵ�ַ
		/// </summary>
		list<string> pubAddress;

		void start();

		void close();
	};
}

