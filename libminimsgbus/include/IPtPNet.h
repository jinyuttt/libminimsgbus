#pragma once
#include <string>
using namespace std;
namespace libminimsgbus
{
    typedef void (*ReceiveMsg)(char*, int);
	class IPtPNet
	{
    protected:
     
    public:
	   /// <summary>
       /// ���ͻ��߽��յ�ַ
       /// </summary>
        string Address;

            /// <summary>
            /// �˿�
            /// </summary>
        int Port;

        ReceiveMsg  revmsg = nullptr;
        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="data"></param>
       virtual void send(char data[])=0;

        /// <summary>
        ///����������ʱ��ַ
        /// </summary>
        /// <param name="addr"></param>
        /// <param name="data"></param>
       virtual  void send(string addr, char data[])=0;

        /// <summary>
        /// ��������
        /// </summary>
       virtual  void start()=0;
	};
}

