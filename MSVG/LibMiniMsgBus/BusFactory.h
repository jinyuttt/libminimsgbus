#pragma once
#include "IMiniMsgBus.h"
#include "MsgDef.h"

#ifdef _MSC_VER
#ifdef FBC_STATIC
#define FBC_API
#elif defined FBC_EXPORT
#define FBC_API __declspec(dllexport)
#else
#define FBC_API __declspec(dllimport)
#endif

#endif
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
	};
}

