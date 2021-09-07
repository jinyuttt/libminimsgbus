#pragma once

#include<string>
#include<list>
#include"NngDataNative.h"
using namespace msgtransport;
using namespace std;
namespace libminimsgbus
{
	class BridgeCore
	{
	private:
		list<NngDataNative*> lstnng;
		list<NngDataNative*> lstnngpub;
	public:
		list<string> recAddress;
		list<string> pubAddress;

		void start();

		void close();
	};
}

