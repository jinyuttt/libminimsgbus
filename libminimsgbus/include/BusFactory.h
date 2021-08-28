#pragma once
#include "IMiniMsgBus.h"
#include "MsgDef.h"
namespace libminimsgbus
{
	class BusFactory
	{
	public:
		static IMiniMsgBus& Create(BusType busType);
		static IMiniMsgBus* CreatePoint(BusType busType);
	};
}

