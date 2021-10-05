#pragma once

#include "IPtPNet.h"

#include "MsgDef.h"
namespace libminimsgbus
{
	class PtpFactory
	{
	public:

		/// <summary>
		/// 创建对象
		/// </summary>
		/// <returns></returns>
		FBC_API static IPtPNet& Create();

		/// <summary>
		/// 创建指针
		/// </summary>
		/// <returns></returns>
		FBC_API 	static IPtPNet* CreatePoint();
	};
}

