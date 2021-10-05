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
        static void addLocal(string topic, msgtopic* ov);

        /// <summary>
        /// ��ȡ���Ķ���
        /// </summary>
        /// <param name="topic"></param>
        /// <returns></returns>
        static msgtopic* getLocal(string topic);
       
    };
};

