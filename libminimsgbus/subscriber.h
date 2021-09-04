#pragma once
#include <map>
#include "SubMgr.h"
#include "msgtopic.h"
using namespace std;
namespace libminimsgbus {
	
	class subscriber
	{
	public:
		subscriber();
		~subscriber();
		void addSubscribe(string topic, msgtopic* msgtopic);
	};
}