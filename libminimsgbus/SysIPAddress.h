#pragma once
#include <iostream>
#include <string>
#include<list>
#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")
#else
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#endif


using namespace std;

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
namespace libminimsgbus
{
 extern	list<std::string> GetNodeAddress()
	{
	 list<string> lst;
#ifdef _WIN32
		PIP_ADAPTER_INFO pAdapterInfo;
		PIP_ADAPTER_INFO pAdapter = NULL;
		DWORD dwRetVal = 0;

		ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
		if (pAdapterInfo == NULL) {
			return lst;
		}

		if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			FREE(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
			if (pAdapterInfo == NULL) {
				return lst;
			}
		}

		std::string ip_address;
		if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
			while (pAdapterInfo)
			{


				pAdapter = pAdapterInfo;

				if (pAdapter) {
					ip_address = pAdapter->IpAddressList.IpAddress.String;
					lst.push_back(ip_address);
				}
				pAdapterInfo = pAdapter->Next;
			}
		
		}

		if (pAdapterInfo) {
			FREE(pAdapterInfo);
		}

		if (!lst.empty()) {
			return lst;
		}
		return  lst;

#else
		struct ifaddrs* ifAddrStruct = NULL;
		struct ifaddrs* ifa = NULL;
		void* tmpAddrPtr = NULL;

		getifaddrs(&ifAddrStruct);
		std::string ip_address;
		for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
			if (!ifa->ifa_addr) {
				continue;
			}
			if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
			  // is a valid IP4 Address
				tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

				if (strcmp(ifa->ifa_name, "lo")) {
					continue;
				}
				else {
					ip_address = addressBuffer;
					if (!ip_address.empty()) {
						break;
					}
				}
			}
		}
		if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
		if (!lst.empty()) {
			return lst;
		}
		else {
			return lst;
		}

#endif
	}
}


