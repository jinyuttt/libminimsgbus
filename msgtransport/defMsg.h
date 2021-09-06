#pragma once
namespace msgtransport
{

     static	std::string nodeguid = " ";

	 //定义函数指针类型
	 typedef void (*ReceiveTopic)(std::string, char*, int);

	struct  MsgBuffer
	{
	public:
		 char* bufdata;
		 int size;
	};

	struct  MsgBody
	{
	public:
		char* bufdata;
		int size;
		std::string head;
	};

   inline	char* ConvertToMsgByte(const char* msg, int size)
	{
	   char* p = new char[size];
		for (int i = 0; i < size; i++)
		{
			p[i] = msg[i];
		}
		return p;
	}
 

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
}
