#pragma once
#include <map>
#include"MsgDef.h"
using namespace std;
namespace libminimsgbus
{
    class SubTable
    {

    private:  //Ĭ�Ϲ���Ϳ������춼��Ҫ��private
        SubTable();
        SubTable(const SubTable& other);
    public:
        static SubTable* GetInstance() {
            static SubTable instance;
            return &instance;
        }

        /// <summary>
        /// ���ڵ㶩�ĵ�ַ
        /// </summary>
        map<string, SubAddressLst> topicPub;

        /// <summary>
        /// ���붩�ķ���ַ
        /// </summary>
        /// <param name="topic">���Ľڵ���Ҫ����</param>
        /// <param name="address">���Ľڵ��ַ</param>
        /// <param name="node">���Ľڵ�ڵ��ʶguid</param>
        /// <returns></returns>
        bool add(string topic, string address, string node);
       
        /// <summary>
        /// ��ȡ���ⶩ�Ľڵ�
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        list<string> getAddress(string topic);
        

        /// <summary>
        /// ��ȡ�����ַ��Ϣ
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        SubAddressLst getAddressLst(string topic);
       

    };
};

