#pragma once
#include <string>
#include<list>
#include"MsgDef.h"
using namespace std;
namespace libminimsgbus
{
    class   MsgLocalNode
    {
    public:
       
        static string LocalAddress;

       
        static int LocalPort;

      
        static string LocalNetAddress;

       
        static string LocalMask;

       
        static string LocalGateway;

      
        static string Netprotocol;

        static string protocol;

      
        static  string GUID;

       

        static  list<NetWorkInfo> LocalAddressFamily;

     
        static bool IsMsgReturn;


        static void GetNetworkInterface();
     

        
    };


   
};
