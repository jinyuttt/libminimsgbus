
#include "PtPNet.h"
#include <iostream>
#include "MsgLocalNode.h"

namespace libminimsgbus
{
	void PtPNet::getRealAddress(string address, string &protol, string &ip, int &port)
	{
		int index = address.find_first_of("//");
		int index1 = address.find_last_of(":");
		
		if (index > -1)
		{
			protol = address.substr(0, index - 1);
		}
		ip = address.substr(index + 2, index1 - index - 2);
		port = std::stoi(address.substr(index1 + 1));
		std::cout << "通信协议:" << protol << "绑定IP:" << ip << "绑定端口 : " << port << std::endl;
		
	}
	void PtPNet::processData()
	{
		thread queue([&]()
		{
				while (true)
				{
					PtpBuffer tmp;
					topicStructs.wait_dequeue(tmp);
					if (revmsg != nullptr)
					{
						revmsg(tmp.data,tmp.len);
					}
					else if(callback!=nullptr)
					{
						callback(tmp.data, tmp.len);
					}
				}
		});
		queue.detach();
	}

	
	void PtPNet::initDataRecive(string localIP, int port)
	{
		NngDataNative native;
		string tmp = "";
		if (MsgLocalNode::protocol.empty())
		{
			tmp = localIP+":"+ to_string(port);
		}
		else
		{
			tmp =  MsgLocalNode::protocol+"://"+ localIP+":"+ to_string(port);
		}
		MsgLocalNode::Netprotocol = native.receive(tmp);
		//
		//Console.WriteLine("LocalNode.Netprotocol:" + LocalNode.Netprotocol);
		std::cout << "LocalNode.Netprotocol:" + MsgLocalNode::Netprotocol << std::endl;
		// getRealAddress(MsgLocalNode::Netprotocol);


		thread rec([&]() 
		{
			//接收数据
			while (true)
			{
				auto data = native.getData();
				PtpBuffer buf{ };
				buf.data = data.bufdata;
				buf.len = data.size;
				topicStructs.enqueue(buf);

			}
		});
		rec.detach();
	}
	void PtPNet::start()
	{
		if (isStart)
		{
			return;
		}
		isStart = true;
		processData();
		initDataRecive(Address, Port);
	}
	void PtPNet::send(char data[])
	{
		NngDataNative native;
		int size = sizeof(data) / sizeof(data[0]);
		if (sendAddress.empty())
		{
			if (MsgLocalNode::protocol.empty())
			{
				sendAddress =  Address+to_string(Port);
			}
			else
			{
				sendAddress =  MsgLocalNode::protocol+"://"+Address+":"+ to_string(Port);
			}
		}
		native.send(sendAddress, data, &size);
	}

	void PtPNet::send(string addr, char data[])
	{
		NngDataNative native;
		int size = sizeof(data) / sizeof(data[0]);
		
		native.send(addr, data, &size);
	}

	
}
