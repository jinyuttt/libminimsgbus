#include "pch.h"
#include "NngDataNative.h"
#include <iostream>
 #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <winsock.h> 
#pragma comment(lib,"ws2_32.lib")
#else
#include <arpa/inet.h> 
#endif
namespace msgtransport
{
	NngDataNative::NngDataNative()
	{
	}
	NngDataNative::~NngDataNative()
	{
		
	}
	char* NngDataNative::send(string address, char bytes[], int* len)
	{
		int size = *len;
		nng::socket req_sock = nng::req::open();

		std::cout << address << std::endl;
		nng::set_opt_recv_timeout(req_sock, 2000);
		nng::set_opt_send_timeout(req_sock, 2000);
		try
		{
			nng::buffer buf(bytes, size);
			req_sock.dial(address.c_str());
			req_sock.send(buf);
			nng::buffer req_buf = req_sock.recv();
			int dlen = req_buf.size();
			len = &dlen;
			auto ret = req_buf.data<char>();
			req_buf.release();
			buf.release();
			return ret;
		}
		catch (nng::exception e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (std::exception ex)
		{
			std::cout << ex.what() << std::endl;
		}
		return nullptr;
	}

	MsgBuffer NngDataNative::getData()
	{
		MsgBuffer item;
		queue.wait_dequeue<MsgBuffer>(item);
		return item;
	}

	string NngDataNative::receive(string url)
	{
		lissock = nng::rep::open();

		try
		{
			lissock.listen(url.c_str());
		}
		catch (nng::exception e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		nng::listener lis(lissock, url.c_str());//关联监听
		lis = nng::make_listener(lissock, url.c_str());
		thread listhread(&NngDataNative::recviceThread, this);
		listhread.detach();
		uint16_t port = 0;
		uint32_t localaddr = 0;
		string ip;
		if (url.substr(url.length() - 2) == ":0" && (url.substr(0, 3) == "tcp" || url.substr(0, 2) == "ws"))
		{

			auto addr = lis.get_opt_addr(nng::to_name(nng::option::local_address));//获取绑定地址

			switch (addr.s_family)
			{
			case   nng_sockaddr_family::NNG_AF_INET:
				port = addr.s_in.sa_port;
				localaddr = htonl(addr.s_in.sa_addr);
				inttoIp(localaddr, ip);
				break;
			case nng_sockaddr_family::NNG_AF_INET6:
				port = addr.s_in6.sa_port;
				ipv6_to_str(ip, addr.s_in6.sa_addr);
				break;
			case nng_sockaddr_family::NNG_AF_UNSPEC:
				port = addr.s_in.sa_port;
				localaddr = htonl(addr.s_in.sa_addr);
				inttoIp(localaddr, ip);
				break;
			default:
				break;
			}
			int index = url.find_last_of(":");
			port = htons(port);
			string	raddr = url.substr(0, index + 1) + std::to_string(port);
			url = raddr;
		}
		return  url;
	}

	

	string NngDataNative::pull(string url)
	{
		pullsock = nng::pull::open();
		try
		{
			lissock.listen(url.c_str());
		}
		catch (nng::exception e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		nng::listener lis(lissock, url.c_str());//关联监听
		lis = nng::make_listener(lissock, url.c_str());
		thread listhread(&NngDataNative::recviceMsg, this);
		listhread.detach();
		uint16_t port = 0;
		uint32_t localaddr = 0;
		string ip;
		if (url.substr(url.length() - 2) == ":0" && (url.substr(0, 3) == "tcp" || url.substr(0, 2) == "ws"))
		{

			auto addr = lis.get_opt_addr(nng::to_name(nng::option::local_address));//获取绑定地址

			switch (addr.s_family)
			{
			case   nng_sockaddr_family::NNG_AF_INET:
				port = addr.s_in.sa_port;
				localaddr = htonl(addr.s_in.sa_addr);
				inttoIp(localaddr, ip);
				break;
			case nng_sockaddr_family::NNG_AF_INET6:
				port = addr.s_in6.sa_port;
				ipv6_to_str(ip, addr.s_in6.sa_addr);
				break;
			case nng_sockaddr_family::NNG_AF_UNSPEC:
				port = addr.s_in.sa_port;
				localaddr = htonl(addr.s_in.sa_addr);
				inttoIp(localaddr, ip);
				break;
			default:
				break;
			}
			int index = url.find_last_of(":");
			port = htons(port);
			string	raddr = url.substr(0, index + 1) + std::to_string(port);
			url = raddr;
		}
		return  url;
	}

	void NngDataNative::push(string address, char bytes[], int* len, string head)
	{
		nng::socket req_sock = nng::push::open();
	
		std::cout << address << std::endl;
		nng::set_opt_recv_timeout(req_sock, 2000);
		nng::set_opt_send_timeout(req_sock, 2000);
		try
		{
			int dlen = head.length() + *len;
			nng::msg msg(dlen);
			nng::view headv(head.data(), head.length());
			nng::view body(bytes, *len);
			msg.header().append(headv);
			msg.body().append(body);
			nng::aio_view aiov;
			aiov.set_msg(msg);
			req_sock.dial(address.c_str());
			req_sock.send(aiov);
			aiov.release_msg();

		}
		catch (nng::exception e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (std::exception ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	MsgBody NngDataNative::getMsg()
	{
		MsgBody item;
		queuemsg.wait_dequeue<MsgBody>(item);
		return item;
	}

	void NngDataNative::publish(string topic, char* data, int len)
	{
		if (!ready)
		{
			ready = true;
			lissock = nng::pub::open();
			lissock.listen(topicurl.data());
		}
		try
		{
			
			int dlen = topic.length() + len;
			nng::msg msg(dlen);
			nng::view headv(topic.data(), topic.length());
			nng::view body(data, len);
			msg.header().append(headv);
			msg.body().append(body);
			nng::aio_view aiov;
			aiov.set_msg(msg);
			lissock.send(aiov);
			aiov.release_msg();

		}
		catch (nng::exception e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (std::exception ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	void NngDataNative::subscribe(string topic)
	{
		if (!subread)
		{
			pullsock = nng::sub::open();
			pullsock.dial(topicurl.data());
			thread listhread(&NngDataNative::recviceMsg, this);
			listhread.detach();
		}
		nng::view cur(topic.data(),topic.size());
		
		nng::sub::set_opt_subscribe(pullsock, cur);
		
		
	}

	void NngDataNative::recviceThread()
	{
		while (!isStop) {
			try
			{
				auto buf = lissock.recv();
				queue.enqueue(MsgBuffer{ (char*)buf.data(),(int)buf.size() });
				nng::buffer repbuf(repMessage(), 4);
				lissock.send(repbuf);
				repbuf.release();
				buf.release();
			}
			catch (nng::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}

	void NngDataNative::recviceMsg()
	{
		while (!isStop) {
			try
			{
				auto buf = pullsock.recv_msg();
				MsgBody msg;
				msg.bufdata =(char*) buf.body().data();
				msg.size = buf.body().size();
				char* head= (char*)buf.header().data();
				msg.head = string(head,buf.header().size());
				queuemsg.enqueue(msg);
			}
			catch (nng::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}

	char* NngDataNative::repMessage()
	{
		if (repMsg == nullptr)
		{
			repMsg = const_cast<char*>(nodeguid.data());
		}
		return repMsg;
	}

	int NngDataNative::inttoIp(uint32_t uiIp, string& strOut)
	{
		strOut.clear();
		unsigned char bN1 = 0, bN2 = 0, bN3 = 0, bN4 = 0;
		char arrIp[32] = { 0 };
		bN1 = (uiIp) & 0xFF;
		bN2 = (uiIp >> 8) & 0xFF;
		bN3 = (uiIp >> 16) & 0xFF;
		bN4 = (uiIp >> 24) & 0xFF;
		sprintf_s(arrIp, sizeof(arrIp), "%d.%d.%d.%d", bN1, bN2, bN3, bN4);
		strOut.assign(arrIp);
		return true;

	}

	int NngDataNative::iptoInt(char* ipStr, uint32_t& ipInt)
	{
		if (ipStr == NULL) { return FALSE; }
		ipInt = 0;
		int tokenInt = 0;
		char* buf;
		char* token = NULL;
		token = strtok_s(ipStr, ".", &buf);
		int i = 3;

		while (token != NULL) {
			tokenInt = atoi(token);
			ipInt += tokenInt * pow(256, i);
			token = strtok_s(NULL, ".", &buf);
			i--;
		}
		if (i != 0) { return false; }
		return true;
	}

	void NngDataNative::ipv6_to_str(string& addr_str, uint32_t ipv6_addr[])
	{
		/* ipv6地址128位，数组ip维数默认为4 */
	/* 输出格式为: A:B:C:D:E:F:G:H. */
		int i;
		unsigned short msw, lsw;

		addr_str[0] = '\0';
		char* addr_str_end_ptr = new char[16];
		for (i = 0; i < 4; i++)
		{
			msw = ipv6_addr[i] >> 16;
			lsw = ipv6_addr[i] & 0x0000ffff;
			addr_str_end_ptr += sprintf_s(addr_str_end_ptr, 16, "%X:%X:", msw, lsw);
		}
		*(addr_str_end_ptr - 1) = '\0';
		addr_str = string(addr_str_end_ptr);
		delete[]addr_str_end_ptr;
	}

	void NngDataNative::ipv6_to_str(string& addr_str, uint8_t ipv6_addr[])
	{
		/* ipv6地址128位，数组ip维数默认为4 */
/* 输出格式为: A:B:C:D:E:F:G:H. */
		int i;
		unsigned short msw, lsw;

		addr_str[0] = '\0';
		char* addr_str_end_ptr = new char[16];
		for (i = 0; i < 16; i++)
		{
			msw = ipv6_addr[i] >> 4;
			lsw = ipv6_addr[i] & 0x000000ff;
			addr_str_end_ptr += sprintf_s(addr_str_end_ptr, 16, "%X:%X:", msw, lsw);
		}
		*(addr_str_end_ptr - 1) = '\0';
		addr_str = string(addr_str_end_ptr);
		delete[]addr_str_end_ptr;
	}

	void NngDataNative::close()
	{
		isStop = true;
		lissock.release();
		pullsock.release();
	}
}
