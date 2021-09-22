#pragma once

#include "IPtPNet.h"

#ifdef _MSC_VER
#ifdef FBC_STATIC
#define FBC_API
#elif defined FBC_EXPORT
#define FBC_API __declspec(dllexport)
#else
#define FBC_API __declspec(dllimport)
#endif
#else
#define FBC_API
#endif
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

