#pragma once
#include "BusFactory.h"
#include "PtpFactory.h"
#include "MsgDef.h"
#include "IPtPNet.h"
#include "BridgeCore.h"
using namespace libminimsgbus;

IMiniMsgBus* msgbus;
IPtPNet* ptp;
BridgeCore mqcore;
extern "C"
{

    /// <summary>
    /// ���ķ�����ʼ��
    /// </summary>
    /// <param name="busType">Э��</param>
    /// <param name="mqurl">mqģʽʱ��ַ</param>
    /// <returns></returns>
    FBC_API  void  initmsg(string busType, string mqurl = "")
    {

        BusType msgBusType = BusType::tcp;
        if (busType == "Inpoc")
        {
            msgBusType = BusType::Inpoc;
        }
        else if (busType == "Ipc")
        {
            msgBusType = BusType::Ipc;
        }
        else if (busType == "mq")
        {
            msgbus = BusFactory::CreateMQ();
            msgbus->url = mqurl;
            return;
        }
        msgbus = BusFactory::CreatePoint(msgBusType);
    }
    /// <summary>
          /// ��������
          /// </summary>
          /// <param name="topic">����</param>
          /// <param name="bytes">����</param>
          /// <param name="len">���ݳ���</param>
          /// <returns></returns>
    FBC_API    uint64_t publish(string topic, char bytes[], int len)
    {

        return msgbus->publish(topic, bytes, len);
    }

    /// <summary>
    /// ����
    /// </summary>
    /// <param name="topic">����</param>
    FBC_API void subscribe(string topic, ReceiveTopic rc)
    {
        msgbus->revmsg = rc;
        msgbus->subscribe(topic);

    }


    /// <summary>
    /// ��������
    /// </summary>
    /// <param name="data"></param>
    /// <param name="len"></param>
    /// <returns></returns>
    FBC_API void send(char data[], int len)
    {
        if (ptp == nullptr)
        {
            ptp = PtpFactory::CreatePoint();
        }

    }

    /// <summary>
    /// ��������
    /// </summary>
    /// <param name="address"></param>
    /// <param name="data"></param>
    /// <param name="len"></param>
    /// <returns></returns>
    FBC_API void send(string address, char data[], int len)
    {
        if (ptp == nullptr)
        {
            ptp = PtpFactory::CreatePoint();
        }
        ptp->send(address, data, len);
    }

    /// <summary>
    /// ��Ե��������
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    FBC_API void start(string address)
    {
        if (ptp == nullptr)
        {
            ptp = PtpFactory::CreatePoint();
        }
        ptp->Address = address;
        ptp->start();
    }

    /// <summary>
    /// MQģʽ���ķ�����ַ�����Զ����
    /// </summary>
    /// <param name="address">��ַ</param>
    /// <returns></returns>
    FBC_API void mqPubish(string address)
    {
        mqcore.pubAddress.push_back(address);
    }

    /// <summary>
    /// MQģʽ���Ľ������ݵ�ַ�����Զ��
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    FBC_API void mqPoll(string address)
    {
        mqcore.recAddress.push_back(address);
    }

    /// <summary>
    /// mqģʽ��������
    /// </summary>
    /// <returns></returns>
    FBC_API void mqStart()
    {
        mqcore.start();
    }

    /// <summary>
    ///mqģʽ�ر�����
    /// </summary>
    /// <returns></returns>
    FBC_API void bridgeClose()
    {
        mqcore.close();
    }
}
