#pragma once
#include <list>
#include <string>
#include <map>
using namespace std;
namespace libminimsgbus
{
    class PubTable
    {

    private:  //默认构造和拷贝构造都需要是private

        map<string, list<string>> topicPub;

        PubTable();
        PubTable(const PubTable& other);


    public:
        static PubTable* GetInstance() {
            static PubTable instance;
            return &instance;
        }
      

        /// <summary>
        /// 保存发布地址
        /// </summary>
        /// <param name="topic">主题</param>
        /// <param name="address">地址</param>
        /// <returns></returns>
        bool add(string topic, string address);
       

        /// <summary>
        /// 获取主题发布地址
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        list<string> getAddress(string topic);
     

        /// <summary>
        /// 主题节点
        /// </summary>
        /// <returns></returns>
        map<string, list<string>> getPairs();
       
    };
}


