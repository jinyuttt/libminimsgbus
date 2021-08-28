#pragma once
#include <string>
#include"concurrentqueue.h"
#include "MsgDef.h"
using namespace std;
using namespace moodycamel;
class ErrorQueue
{
 
public:
   
    string Topic;

   
    ConcurrentQueue<Records>  queueError;

    
    bool IsComlepte;
};