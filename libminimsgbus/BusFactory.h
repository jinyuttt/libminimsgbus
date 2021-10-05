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
		/// 创建引用对象
		/// </summary>
		/// <param name="busType">枚举</param>
		/// <returns></returns>
		FBC_API	static IMiniMsgBus& Create(BusType busType);

		/// <summary>
		/// 创建对象指针
		/// </summary>
		/// <param name="busType">枚举</param>
		/// <returns></returns>
		FBC_API static IMiniMsgBus* CreatePoint(BusType busType);

		/// <summary>
		/// 创建MQ模式的订阅发布
		/// </summary>
		/// <returns></returns>
		FBC_API static IMiniMsgBus* CreateMQ();


	};
}

