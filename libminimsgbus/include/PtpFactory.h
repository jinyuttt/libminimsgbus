#pragma once

#include "IPtPNet.h"
namespace libminimsgbus
{
	class PtpFactory
	{
	public:
		static IPtPNet& Create();
		static IPtPNet* CreatePoint();
	};
}

