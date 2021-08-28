#pragma once
#include <string>
using namespace std;
namespace libminimsgbus
{
    typedef void (*ReceiveMsg)(char*, int);
	class IPtPNet
	{
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
    public:
        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="data"></param>
       virtual void send(char data[])=0;

        /// <summary>
        ///发送数据临时地址
        /// </summary>
        /// <param name="addr"></param>
        /// <param name="data"></param>
       virtual  void send(string addr, char data[])=0;

        /// <summary>
        /// 启动接收
        /// </summary>
       virtual  void start()=0;
	};
}

