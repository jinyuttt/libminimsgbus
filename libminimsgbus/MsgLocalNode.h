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
        /// 本地IP
        /// </summary>
        static string LocalAddress;

        /// <summary>
        /// 本地端口
        /// </summary>
        static int LocalPort;

        /// <summary>
        /// 本地网络地址
        /// </summary>
        static string LocalNetAddress;

       
        /// <summary>
        /// mask
        /// </summary>
        static string LocalMask;

        /// <summary>
        /// 本地网关
        /// </summary>
        static string LocalGateway;

        /// <summary>
        /// 网络协议
        /// </summary>
        static string Netprotocol;

        /// <summary>
        ///通讯协议
        /// </summary>
        static string protocol;

        /// <summary>
        /// 本地GUID
        /// </summary>
        static  string GUID;

       
        /// <summary>
        /// 本地所有通讯
        /// </summary>
        static  list<NetWorkInfo> LocalAddressFamily;

        /// <summary>
        /// 需要信息返回
        /// </summary>
        static bool IsMsgReturn;

        /// <summary>
        /// 远端通讯地址，组播不通时
        /// </summary>
        static list<string> remote;

        /// <summary>
        /// 获取本机IP
        /// </summary>
        static void GetNetworkInterface();
     

        
    };


   
};
