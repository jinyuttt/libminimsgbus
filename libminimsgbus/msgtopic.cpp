
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
        //释放对象相当于注销主题
        suber->remove(this);
        delete suber;
    }
    void msgtopic::subscribe(string topic)
    {
        suber->addSubscribe(topic, this);
    }
    
    int64_t msgtopic::publish(string topic, char* msg,int len)
    {
      
        return   PubMgr::GetInstance()->send(topic, msg,len);
    }

     void msgtopic::unsubscribe(string topic)
    {
         return  suber->unsubscribe(topic);
    }
    void msgtopic::addtopic(string topic, char* msg,int len)
	{
        try

        {
            if (rectopic != nullptr)
            {
                //调用ObjSubMgr
                rectopic(topic, msg, len);
            }
           
        }
        catch (std::exception e)
        {
            std::cout << e.what() << std::endl;
        }
	}
}
