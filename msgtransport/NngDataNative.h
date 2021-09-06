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
		/// req����
		/// </summary>
		BlockingConcurrentQueue<MsgBuffer> queue;

		/// <summary>
		/// pull,sub����
		/// </summary>
		BlockingConcurrentQueue<MsgBody> queuemsg;
		std::atomic<bool>  ready = std::atomic_bool(false); ;
		std::atomic<bool>  subread;
		/// <summary>
		///�߳̽���
		/// </summary>
		void recviceThread();

		/// <summary>
		/// �߳̽�������
		/// </summary>
		void recviceMsg();
		/// <summary>
		/// �ظ�����
		/// </summary>
		/// <returns></returns>
		 char* repMessage();
		
		 /// <summary>
		 /// ����תIP
		 /// </summary>
		 /// <param name="uiIp"></param>
		 /// <param name="strOut"></param>
		 /// <returns></returns>
		 int inttoIp(uint32_t uiIp, string& strOut);

		 /// <summary>
		 /// IPת������
		 /// </summary>
		 /// <param name="ipStr"></param>
		 /// <param name="ipInt"></param>
		 /// <returns></returns>
		 int iptoInt(char* ipStr, uint32_t& ipInt);

		 /// <summary>
		 /// IP6ת�ַ���
		 /// </summary>
		 /// <param name="addr_str"></param>
		 /// <param name="ipv6_addr"></param>
		 void ipv6_to_str(string& addr_str, uint32_t ipv6_addr[]);
		 void ipv6_to_str(string& addr_str, uint8_t ipv6_addr[]);
	public:

		/// <summary>
		/// ���ķ���ʹ�õ�ַ
		/// </summary>
		string topicurl;
		NngDataNative();
		~NngDataNative();
		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="address">��ַ</param>
		/// <param name="bytes">����</param>
		/// <param name="len">���ݳ���</param>
		/// <returns></returns>
		FBC_API  char* send(string address, char bytes[], int* len);

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="address">���ص�ַ</param>
		/// <returns></returns>
		FBC_API  string  receive(string address);

		/// <summary>
		/// ��������
		/// </summary>
		/// <returns></returns>
		FBC_API  MsgBuffer getData();

		

		/// <summary>
		/// ��ȡ����
		/// </summary>
		/// <param name="address">���յ�ַ</param>
		/// <returns></returns>
		FBC_API  string  pull(string address);

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="address">��ַ</param>
		/// <param name="head">ͷ��ʶ</param>
		/// <param name="bytes"></param>
		/// <param name="len"></param>
		FBC_API  void  push(string address, char bytes[], int* len, string head="");

		/// <summary>
		/// ��ȡ�ⲿ����
		/// </summary>
		/// <returns></returns>
		FBC_API  MsgBody getMsg();

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="topic">����</param>
		/// <param name="data">����</param>
		/// <param name="len">����</param>
		FBC_API  void  publish(string topic, char* data, int len);

		/// <summary>
		/// ����
		/// </summary>
		/// <param name="topic"></param>
		FBC_API  void  subscribe(string topic);

		/// <summary>
		/// �رս���
		/// </summary>
		FBC_API void close();
	};
}

