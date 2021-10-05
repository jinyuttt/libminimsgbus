#pragma once
#include "IMiniMsgBus.h"
#include "MsgStruct.h"
#include "MsgDef.h"

namespace libminimsgbus
{
	class BusFactory
	{
	public:

		/// <summary>
		/// �������ö���
		/// </summary>
		/// <param name="busType">ö��</param>
		/// <returns></returns>
		FBC_API	static IMiniMsgBus& Create(BusType busType);

		/// <summary>
		/// ��������ָ��
		/// </summary>
		/// <param name="busType">ö��</param>
		/// <returns></returns>
		FBC_API static IMiniMsgBus* CreatePoint(BusType busType);

		/// <summary>
		/// ����MQģʽ�Ķ��ķ���
		/// </summary>
		/// <returns></returns>
		FBC_API static IMiniMsgBus* CreateMQ();


	};
}

