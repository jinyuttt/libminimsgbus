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
        /// ���붩�Ķ���
        /// </summary>
        /// <param name="topic"></param>
        /// <param name="ov"></param>
        static void AddLocal(string topic, msgtopic* ov);

        /// <summary>
        /// ��ȡ���Ķ���
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        static msgtopic* GetLocal(string topic);
       
    };
};

