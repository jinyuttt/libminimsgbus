#pragma once
#include <string>
#include <map>
#include<list>
using namespace std;
struct  PubRecords
{

    /// <summary>
    /// 消息ID
    /// </summary>
     int MsgId;

    /// <summary>
    /// 成功次数
    /// </summary>
     int SucessNum;

    /// <summary>
    /// 失败次数
    /// </summary>
     int FaildNum;
};

/// <summary>
   /// 记录发送的数据,处理异常
   /// </summary>
struct Records
{
    /// <summary>
    /// 地址+数据
    /// </summary>
     map<string, char*> Record;

    /// <summary>
    /// 主题
    /// </summary>
     string Topic;

    /// <summary>
    /// 消息ID
    /// </summary>
     uint64_t MsgId;
};

/// <summary>
   /// 订阅节点地址
   /// </summary>
struct SubAddress
{
public:
    /// <summary>
    /// 使用地址
    /// </summary>
    string Address;

        /// <summary>
        /// 所有地址
        /// </summary>
    list<string> AllAddress;


        /// <summary>
        /// 认为异常的地址
        /// </summary>
    list<string> ErrorAddress;


        /// <summary>
        /// 节点标识guid
        /// </summary>
    string NodeFlage;

        /// <summary>
        /// 次数
        /// </summary>
    int NumAll=0;
};

/// <summary>
/// 主题订阅地址
/// </summary>
struct SubAddressLst
{
    /// <summary>
    /// 所有节点地址
    /// </summary>
    list<SubAddress> SubAddresses;

        /// <summary>
        /// 控制地址加入
        /// </summary>
    list<string> LstAddress;
};


class TopicStruct
{
public:
    /// <summary>
    /// 消息ID
    /// </summary>
    int64_t MsgId;

    /// <summary>
    /// 消息节点标识
    /// </summary>
    string MsgNode;

    /// <summary>
    /// 主题
    /// </summary>
    string Topic;

    /// <summary>
    /// 消息标识0是消息1是订阅
    /// </summary>
    char Flage='-1';

    /// <summary>
    /// 消息内容
    /// </summary>
    char* Msg;

    int msglen;

};
struct NetWorkInfo
{
public:
    /// <summary>
     /// IP4网络
     /// </summary>
    string IPV4;

    /// <summary>
    /// IP6网络
    /// </summary>
    string IPV6;

    /// <summary>
    /// 掩码
    /// </summary>
    string Mask;

    /// <summary>
    /// 路由地址
    /// </summary>
    string GatewayAddress;

    /// <summary>
    /// Dns地址
    /// </summary>
    string DnsAddress;
};
  enum class BusType
{
     
    /// <summary>
    /// 进程内
    /// </summary>
    Inpoc,

    /// <summary>
    /// 进程间
    /// </summary>
    Ipc,

    /// <summary>
    /// 网络
    /// </summary>
    tcp
};