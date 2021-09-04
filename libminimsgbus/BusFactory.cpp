#include "pch.h"
#include "BusFactory.h"
#include "InprocMsgBus.h"
#include "IpcMsgBus.h"
#include "NetMsgBus.h"
namespace libminimsgbus
{
	IMiniMsgBus& BusFactory::Create(BusType busType)
	{
       
        InprocMsgBus inp;
        IpcMsgBus ipc;
        NetMsgBus bus;
        IMiniMsgBus& msginp= inp;
        IMiniMsgBus& msgipc = ipc;
        IMiniMsgBus& msgBus = bus;
       
        switch (busType)
        {
        case BusType::Inpoc:
           
            return inp;
            break;
        case BusType::Ipc:
            return ipc;
            break;
        case BusType::tcp:
            return msgBus;
            break;
        default:
            return msgBus;
            break;

        }
        return msgBus;
	}
    IMiniMsgBus* BusFactory::CreatePoint(BusType busType)
    {
        IMiniMsgBus* msgBus = nullptr;
        switch (busType)
        {
        case BusType::Inpoc:

            msgBus = new InprocMsgBus();
            break;
        case BusType::Ipc:
            msgBus = new IpcMsgBus();
            break;
        case BusType::tcp:
             msgBus=new  NetMsgBus();
            break;
        default:
            msgBus = new  NetMsgBus();
            break;

        }
        return msgBus;
    }
}