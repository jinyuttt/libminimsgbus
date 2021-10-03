
#include "MsgLocalNode.h"
#include "SysIPAddress.h"
#include "MsgDef.h"
#include "Util.h"
using namespace std;
namespace libminimsgbus
{
	std::string MsgLocalNode::GUID = Util::generate_hex(16);
	std::string MsgLocalNode::LocalAddress = "*";
	std::list<NetWorkInfo> MsgLocalNode::LocalAddressFamily;
	std::string MsgLocalNode::LocalGateway = "";
	std::string MsgLocalNode::LocalMask = "";
	std::string MsgLocalNode::LocalNetAddress = "";
	int MsgLocalNode::LocalPort = 0;

	std::string MsgLocalNode::Netprotocol = "";
	std::string MsgLocalNode::protocol = "tcp";
	std::list<string> MsgLocalNode::remote;
	void MsgLocalNode::GetNetworkInterface()
	{
		if (!LocalAddressFamily.empty())
		{
			return;
		}
		auto lstAddress = GetNodeAddress();
		for (auto ip : lstAddress)
		{
			if (ip == "0.0.0.0")
			{
				continue;
			}
			NetWorkInfo tmp;
			tmp.IPV4 = ip;
			LocalAddressFamily.push_back(tmp);
		}
	
	}

}
