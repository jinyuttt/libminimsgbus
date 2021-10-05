#pragma once

#include "IPtPNet.h"

#include "MsgDef.h"
namespace libminimsgbus
{
	class PtpFactory
	{
	public:

		/// <summary>
		/// ��������
		/// </summary>
		/// <returns></returns>
		FBC_API static IPtPNet& Create();

		/// <summary>
		/// ����ָ��
		/// </summary>
		/// <returns></returns>
		FBC_API 	static IPtPNet* CreatePoint();
	};
}

