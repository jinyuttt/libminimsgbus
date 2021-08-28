#pragma once
#include <map>
#include"MsgDef.h"
using namespace std;
namespace libminimsgbus
{
    class SubTable
    {

    private:  //默认构造和拷贝构造都需要是private
        SubTable();
        SubTable(const SubTable& other);
    public:
        static SubTable* GetInstance() {
            static SubTable instance;
            return &instance;
        }

        /// <summary>
        /// 本节点订阅地址
        /// </summary>
        map<string, SubAddressLst> topicPub;

        /// <summary>
        /// 加入订阅方地址
        /// </summary>
        /// <param name="topic">订阅节点需要主题</param>
        /// <param name="address">订阅节点地址</param>
        /// <param name="node">订阅节点节点标识guid</param>
        /// <returns></returns>
        bool add(string topic, string address, string node);
       
        /// <summary>
        /// 获取主题订阅节点
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        list<string> getAddress(string topic);
        

        /// <summary>
        /// 获取主题地址信息
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        SubAddressLst getAddressLst(string topic);
       

    };
};

