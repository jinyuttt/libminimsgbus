#pragma once
#include <string>
#include <map>
#include<list>
using namespace std;
struct  PubRecords
{

    /// <summary>
    /// ��ϢID
    /// </summary>
     int MsgId;

    /// <summary>
    /// �ɹ�����
    /// </summary>
     int SucessNum;

    /// <summary>
    /// ʧ�ܴ���
    /// </summary>
     int FaildNum;
};

/// <summary>
   /// ��¼���͵�����,�����쳣
   /// </summary>
struct Records
{
    /// <summary>
    /// ��ַ+����
    /// </summary>
     map<string, char*> Record;

    /// <summary>
    /// ����
    /// </summary>
     string Topic;

    /// <summary>
    /// ��ϢID
    /// </summary>
     uint64_t MsgId;
};

/// <summary>
   /// ���Ľڵ��ַ
   /// </summary>
struct SubAddress
{
public:
    /// <summary>
    /// ʹ�õ�ַ
    /// </summary>
    string Address;

        /// <summary>
        /// ���е�ַ
        /// </summary>
    list<string> AllAddress;


        /// <summary>
        /// ��Ϊ�쳣�ĵ�ַ
        /// </summary>
    list<string> ErrorAddress;


        /// <summary>
        /// �ڵ��ʶguid
        /// </summary>
    string NodeFlage;

        /// <summary>
        /// ����
        /// </summary>
    int NumAll=0;
};

/// <summary>
/// ���ⶩ�ĵ�ַ
/// </summary>
struct SubAddressLst
{
    /// <summary>
    /// ���нڵ��ַ
    /// </summary>
    list<SubAddress> SubAddresses;

        /// <summary>
        /// ���Ƶ�ַ����
        /// </summary>
    list<string> LstAddress;
};


class TopicStruct
{
public:
    /// <summary>
    /// ��ϢID
    /// </summary>
    int64_t MsgId;

    /// <summary>
    /// ��Ϣ�ڵ��ʶ
    /// </summary>
    string MsgNode;

    /// <summary>
    /// ����
    /// </summary>
    string Topic;

    /// <summary>
    /// ��Ϣ��ʶ0����Ϣ1�Ƕ���
    /// </summary>
    char Flage='-1';

    /// <summary>
    /// ��Ϣ����
    /// </summary>
    char* Msg;

    int msglen;

};
struct NetWorkInfo
{
public:
    /// <summary>
     /// IP4����
     /// </summary>
    string IPV4;

    /// <summary>
    /// IP6����
    /// </summary>
    string IPV6;

    /// <summary>
    /// ����
    /// </summary>
    string Mask;

    /// <summary>
    /// ·�ɵ�ַ
    /// </summary>
    string GatewayAddress;

    /// <summary>
    /// Dns��ַ
    /// </summary>
    string DnsAddress;
};
  enum class BusType
{
     
    /// <summary>
    /// ������
    /// </summary>
    Inpoc,

    /// <summary>
    /// ���̼�
    /// </summary>
    Ipc,

    /// <summary>
    /// ����
    /// </summary>
    tcp
};