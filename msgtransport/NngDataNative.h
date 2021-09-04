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
		///�߳̽���
		/// </summary>
		void recviceThread();

		/// <summary>
		/// �ظ�����
		/// </summary>
		/// <returns></returns>
		 char* repMessage();

		 int inttoIp(uint32_t uiIp, string& strOut);
		 int iptoInt(char* ipStr, uint32_t& ipInt);
		 void ipv6_to_str(string& addr_str, uint32_t ipv6_addr[]);
		 void ipv6_to_str(string& addr_str, uint8_t ipv6_addr[]);
	public:

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="address">��ַ</param>
		/// <param name="bytes">����</param>
		/// <param name="len">���ݳ���</param>
		/// <returns></returns>
		FBC_API  char* send(string address, char bytes[], int* len);

		/// <summary>
		/// ��������
		/// </summary>
		/// <returns></returns>
		FBC_API  MsgBuffer getData();

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="address">���ص�ַ</param>
		/// <returns></returns>
		FBC_API  string  receive(string address);

		/// <summary>
		/// �رս���
		/// </summary>
		FBC_API void close();
	};
}

