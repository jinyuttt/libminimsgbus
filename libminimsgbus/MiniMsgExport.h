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
    /// 订阅发布初始化
    /// </summary>
    /// <param name="busType">协议</param>
    /// <param name="mqurl">mq模式时地址</param>
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
          /// 发布数据
          /// </summary>
          /// <param name="topic">主题</param>
          /// <param name="bytes">数据</param>
          /// <param name="len">数据长度</param>
          /// <returns></returns>
    FBC_API    uint64_t publish(string topic, char bytes[], int len)
    {

        return msgbus->publish(topic, bytes, len);
    }

    /// <summary>
    /// 订阅
    /// </summary>
    /// <param name="topic">主题</param>
    FBC_API void subscribe(string topic, ReceiveTopic rc)
    {
        msgbus->revmsg = rc;
        msgbus->subscribe(topic);

    }


    /// <summary>
    /// 发送数据
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
    /// 发送数据
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
    /// 点对点接受数据
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
    /// MQ模式中心发布地址（可以多个）
    /// </summary>
    /// <param name="address">地址</param>
    /// <returns></returns>
    FBC_API void mqPubish(string address)
    {
        mqcore.pubAddress.push_back(address);
    }

    /// <summary>
    /// MQ模式中心接受数据地址，可以多个
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    FBC_API void mqPoll(string address)
    {
        mqcore.recAddress.push_back(address);
    }

    /// <summary>
    /// mq模式开启中心
    /// </summary>
    /// <returns></returns>
    FBC_API void mqStart()
    {
        mqcore.start();
    }

    /// <summary>
    ///mq模式关闭中心
    /// </summary>
    /// <returns></returns>
    FBC_API void bridgeClose()
    {
        mqcore.close();
    }
}
