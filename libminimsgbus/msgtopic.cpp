
#include "msgtopic.h"
#include "subscriber.h"
namespace libminimsgbus
{
    msgtopic::msgtopic()
    {
        suber = new subscriber();
    }
    msgtopic::~msgtopic()
    {
        delete suber;
    }
    void msgtopic::subscribe(string topic)
    {
        suber->addSubscribe(topic, this);
    }
    
    int64_t msgtopic::publish(string topic, char msg[])
    {
      
        return   PubMgr::GetInstance()->send(topic, msg);
    }
    void msgtopic::addtopic(string topic, char msg[],int len)
	{
          if (rectopic != nullptr)
          {
              rectopic(topic, msg, len);
          }
      
	}
}
