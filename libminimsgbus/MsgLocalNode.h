#pragma once
#include <string>
#include<list>
#include "MsgStruct.h"
using namespace std;
namespace libminimsgbus
{
    class   MsgLocalNode
    {
    public:
       
        /// <summary>
        /// ����IP
        /// </summary>
        static string LocalAddress;

        /// <summary>
        /// ���ض˿�
        /// </summary>
        static int LocalPort;

        /// <summary>
        /// ���������ַ
        /// </summary>
        static string LocalNetAddress;

       
        /// <summary>
        /// mask
        /// </summary>
        static string LocalMask;

        /// <summary>
        /// ��������
        /// </summary>
        static string LocalGateway;

        /// <summary>
        /// ����Э��
        /// </summary>
        static string Netprotocol;

        /// <summary>
        ///ͨѶЭ��
        /// </summary>
        static string protocol;

        /// <summary>
        /// ����GUID
        /// </summary>
        static  string GUID;

       
        /// <summary>
        /// ��������ͨѶ
        /// </summary>
        static  list<NetWorkInfo> LocalAddressFamily;

        /// <summary>
        /// ��Ҫ��Ϣ����
        /// </summary>
        static bool IsMsgReturn;

        /// <summary>
        /// Զ��ͨѶ��ַ���鲥��ͨʱ
        /// </summary>
        static list<string> remote;

        /// <summary>
        /// ��ȡ����IP
        /// </summary>
        static void GetNetworkInterface();
     

        
    };


   
};
