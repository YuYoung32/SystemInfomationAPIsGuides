#include<WinSock2.h>
#include <iphlpapi.h>
#include<WS2tcpip.h>
#pragma comment(lib,"iphlpapi.lib")

#include <vector>

void PrintAdaptersInfo();

void PrintNetcardIPv4Addr(void* pIp);
void PrintNetcardMACAddr(void* pIp);
void PrintNetcardStatus(void* pIp);
bool IsLoopbackIPv4(void* head)
{
	auto unicastAddress =
		((PIP_ADAPTER_ADDRESSES)head)->FirstUnicastAddress;
	auto address = ((sockaddr_in*)unicastAddress->Address.lpSockaddr)->sin_addr.S_un.S_un_b;

	if (address.s_b1 == 127 && address.s_b2 == 0 && address.s_b3 == 0 && address.s_b4 == 1)
	{
		return true;
	}
	return false;
}
bool IsLoopbackIPv6(const BYTE ipv6Address[16])
{

	for (size_t i = 0; i < 15; i++)
	{
		if (ipv6Address[i] != 0)
		{
			return false;
		}
	}
	if (ipv6Address[15] == 1)
	{
		return true;
	}
	return false;
}

void PrintAdaptersInfo()
{
	PMIB_IFTABLE pMibIfTable;
	// get space
	ULONG size = 0;
	int success = GetAdaptersAddresses(
		AF_INET,
		GAA_FLAG_INCLUDE_PREFIX,
		NULL,
		nullptr,
		&size);
	std::vector<BYTE> pIpAdapter;
	pIpAdapter.resize(size);

	GetAdaptersAddresses(
		AF_INET,
		GAA_FLAG_INCLUDE_PREFIX,
		NULL,
		(PIP_ADAPTER_ADDRESSES)pIpAdapter.data(),
		&size);
	int cnt = 1;
	for (auto head = (PIP_ADAPTER_ADDRESSES)pIpAdapter.data();
		head != NULL; 
		head = head->Next)
	{
		// skip virtual loopback netcard
		if (IsLoopbackIPv4(head))
		{
			continue;
		}
		printf("Netcard %d\n", cnt++);
		PrintNetcardIPv4Addr(head);
		PrintNetcardMACAddr(head);
		PrintNetcardStatus(head);
		auto UpdateTransmitBits = [&](DWORD& up, DWORD& down) {
			// get system interface table
			// get space
			ULONG size = 0;
			GetIfTable(nullptr, &size, false);
			// dynamic mem
			std::vector<BYTE> pIfTable;
			pIfTable.resize(size);
			pMibIfTable = (PMIB_IFTABLE)pIfTable.data();
			GetIfTable(pMibIfTable, &size, false);

			// there ara many system interfaces, compare the MAC address to select the netcard 
			for (size_t i = 0; i < pMibIfTable->dwNumEntries; i++)
			{
				auto& st = pMibIfTable->table[i];
				constexpr auto MAC_ADDR_BYTE_LENGTH = 6;
				if (!memcmp((void*)head->PhysicalAddress,
					(void*)st.bPhysAddr,
					MAC_ADDR_BYTE_LENGTH))
				{
					up = st.dwOutOctets;
					down = st.dwInOctets;
					break;
				}
			}
		};
		DWORD beforeUP, afterUP, beforeDown, afterDown;
		UpdateTransmitBits(beforeUP, beforeDown);
		constexpr auto GAP_MS = 1000;
		Sleep(GAP_MS);
		UpdateTransmitBits(afterUP, afterDown);
		printf("download rate(KB/s): %f, upload rate(KB/s): %f \n",
			1.0 * (afterDown - beforeDown) / 8 / 1024,
			1.0 * (afterUP - beforeUP) / 8 / 1024);
		printf("\n");
	}
}

void PrintNetcardIPv4Addr(void* pIp)
{
	printf("IPv4 Address: ");
	std::vector<BYTE> IPv4Array;
	for (auto head = ((PIP_ADAPTER_ADDRESSES)pIp)->FirstUnicastAddress;
		head != NULL; head = head->Next)
	{
		auto address = ((sockaddr_in*)head->Address.lpSockaddr)->sin_addr.S_un.S_un_b;
		printf("\t %d.%d.%d.%d",
			address.s_b1,
			address.s_b2,
			address.s_b3,
			address.s_b4);
	}
	printf("\n");
}

void PrintNetcardMACAddr(void* pIp)
{
	std::vector<BYTE> macAddr;
	printf("MAC Address: ");
	auto pIpAdapterAddress = (PIP_ADAPTER_ADDRESSES)pIp;
	printf("\t %.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
		pIpAdapterAddress->PhysicalAddress[0],
		pIpAdapterAddress->PhysicalAddress[1],
		pIpAdapterAddress->PhysicalAddress[2],
		pIpAdapterAddress->PhysicalAddress[3],
		pIpAdapterAddress->PhysicalAddress[4],
		pIpAdapterAddress->PhysicalAddress[5]);
	printf("\n");
}

void PrintNetcardStatus(void* pIp)
{
	auto pIpAdapterAddress = (PIP_ADAPTER_ADDRESSES)pIp;
	if (pIpAdapterAddress->OperStatus == IfOperStatusUp)
	{
		printf("status OK\n");
	}
	else
	{
		printf("status BAD\n");
	}
}