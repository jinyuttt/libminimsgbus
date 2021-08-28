#pragma once
#include <string>
#include<map>
#include "msgtopic.h"
using namespace std;
namespace libminimsgbus
{
    class LocalTopic
    {
        static  map<string, msgtopic*> localSub;
    public:

        /// <summary>
        /// 加入订阅对象
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="ov"></param>
        static void AddLocal(string topic, msgtopic* ov);

        /// <summary>
        /// 获取订阅对象
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        static msgtopic* GetLocal(string topic);
       
    };
};

