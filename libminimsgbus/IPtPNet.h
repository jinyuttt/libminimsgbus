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
       /// ���ͻ��߽��յ�ַ
       /// </summary>
        string Address;

        /// <summary>
        /// �˿�
        /// </summary>
        int Port;

        ReceiveMsg  revmsg = nullptr;

        function<void(char*, int)> callback;

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="data"></param>
       virtual void send(char data[],int len)=0;

        /// <summary>
        ///����������ʱ��ַ
        /// </summary>
        /// <param name="addr"></param>
        /// <param name="data"></param>
       virtual  void send(string addr, char data[],int len)=0;

        /// <summary>
        /// ��������
        /// </summary>
       virtual  void start()=0;
	};
}

