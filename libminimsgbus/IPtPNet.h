#pragma once
#include <string>
#include <functional>
using namespace std;
namespace libminimsgbus
{
    typedef void (*ReceiveMsg)(char*, int);
	class IPtPNet
	{
   
    public:
    protected:
        /// <summary>
       /// 发送或者接收地址
       /// </summary>
        string Address;

        /// <summary>
        /// 端口
        /// </summary>
        int Port;

        ReceiveMsg  revmsg = nullptr;

        function<void(char*, int)> callback;

        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="data"></param>
       virtual void send(char data[],int len)=0;

        /// <summary>
        ///发送数据临时地址
        /// </summary>
        /// <param name="addr"></param>
        /// <param name="data"></param>
       virtual  void send(string addr, char data[],int len)=0;

        /// <summary>
        /// 启动接收
        /// </summary>
       virtual  void start()=0;
	};
}

