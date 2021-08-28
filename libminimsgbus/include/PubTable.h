#pragma once
#include <list>
#include <string>
#include <map>
using namespace std;
namespace libminimsgbus
{
    class PubTable
    {

    private:  //Ĭ�Ϲ���Ϳ������춼��Ҫ��private

        map<string, list<string>> topicPub;

        PubTable();
        PubTable(const PubTable& other);


    public:
        static PubTable* GetInstance() {
            static PubTable instance;
            return &instance;
        }
      

        /// <summary>
        /// ���淢����ַ
        /// </summary>
        /// <param name="topic">����</param>
        /// <param name="address">��ַ</param>
        /// <returns></returns>
        bool add(string topic, string address);
       

        /// <summary>
        /// ��ȡ���ⷢ����ַ
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        list<string> getAddress(string topic);
     

        /// <summary>
        /// ����ڵ�
        /// </summary>
        /// <returns></returns>
        map<string, list<string>> getPairs();
       
    };
}


