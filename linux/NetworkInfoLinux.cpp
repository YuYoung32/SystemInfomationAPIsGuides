// for ifaddrs, getifaddrs
#include <ifaddrs.h>

// for IF_LOOPBACK
#include <net/if.h>

// for ioctl
#include <sys/ioctl.h>

// for sock_addr_in
#include <arpa/inet.h>

// for rtnl_link_stats
#include <linux/if_link.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>

void PrintAdaptersInfo();

// each NIC information is stored separately in a linked table,
// so we need to gather them to print
typedef std::array<uint8_t, 4> T_IPv4Addr;
typedef std::array<uint8_t, 16> T_IPv6Addr;
constexpr auto MAC_ADDR_LENGTH = 6;
constexpr auto IPv4_ADDR_LENGTH = 4;
constexpr auto IPv6_ADDR_LENGTH = 16;

struct T_NICInfo
{
	std::string name;
	std::array<uint8_t, MAC_ADDR_LENGTH> MACAddr;
	std::vector<T_IPv4Addr> IPv4Addr;
	std::vector<T_IPv6Addr> IPv6Addr;
	int status;
	float uploadRate;
	float downloadRate;
};

void PrintAdaptersInfo()
{
	struct ifaddrs* head = nullptr;
	int success = getifaddrs(&head);

	// for ioctl get MAC addr
	int socketFd = socket(AF_INET, SOCK_DGRAM, 0);

	// for we can modify the NIC info in the map
	std::map<std::string, std::shared_ptr<struct T_NICInfo>> NICs;

	for (struct ifaddrs* curAddr = head; curAddr != nullptr; curAddr = curAddr->ifa_next)
	{
		if ((curAddr->ifa_flags & IFF_LOOPBACK) != 0)
		{
			// skip loopback
			continue;
		}

		auto&& [it, ok] = NICs.try_emplace(curAddr->ifa_name, std::make_shared<T_NICInfo>());
		auto& info = it->second;

		if (ok)
		{
			// there is no exist NIC info in the map
			// NIC name
			{
				info->name = curAddr->ifa_name;
			}

			// // NIC MAC addr
			{
				struct ifreq nic;
				memcpy(nic.ifr_ifrn.ifrn_name, info->name.c_str(), info->name.size() + 1);
				if (ioctl(socketFd, SIOCGIFHWADDR, &nic) >= 0)
				{
					memcpy(info->MACAddr.data(), nic.ifr_hwaddr.sa_data, MAC_ADDR_LENGTH);
				}
			}

			// NIC status
			{
				info->status = (curAddr->ifa_flags & IFF_UP) == 0;
			}
		}

		if (curAddr->ifa_addr != NULL)
		{
			auto family = curAddr->ifa_addr->sa_family;
			// NIC IPv4
			if (family == AF_INET)
			{
				T_IPv4Addr address;
				memcpy(address.data(), (void*)(&((struct sockaddr_in*)curAddr->ifa_addr)->sin_addr), IPv4_ADDR_LENGTH);
				info->IPv4Addr.push_back(address);
			}
			// NIC IPv6
			else if (family == AF_INET6)
			{
				T_IPv6Addr address;
				memcpy(address.data(), (void*)(&((struct sockaddr_in6*)(curAddr->ifa_addr))->sin6_addr), IPv6_ADDR_LENGTH);
				info->IPv6Addr.push_back(address);
			}
			// NIC upload/download rates
			// notes: upload/download bytes count can be obtained from file /proc/net/dev
			else if (family == AF_PACKET && curAddr->ifa_data != NULL)
			{
				struct rtnl_link_stats* stats = (struct rtnl_link_stats*)curAddr->ifa_data;
				uint32_t preDownloadBytes = stats->rx_bytes;
				uint32_t preUploadBytes = stats->tx_bytes;
				constexpr auto SLEEP_SECONDS = 1;
				sleep(SLEEP_SECONDS);
				stats = (struct rtnl_link_stats*)curAddr->ifa_data;
				uint32_t curDownloadBytes = stats->rx_bytes;
				uint32_t curUploadBytes = stats->tx_bytes;

				info->downloadRate = 1.0 * (curDownloadBytes - preDownloadBytes) / 1024 / SLEEP_SECONDS;
				info->uploadRate = 1.0 * (curUploadBytes - preUploadBytes) / 1024 / SLEEP_SECONDS;
			}
		}
	}

	for (auto& it : NICs)
	{
		printf("adapter name: %s\n", it.second->name.c_str());
		if (it.second->status == 0)
		{
			printf("\tstatus: OK\n");
		}
		else
		{
			printf("\tstatus: BAD\n");
		}
		printf("\tIPv4:");
		for (auto& ipv4 : it.second->IPv4Addr)
		{
			printf("\t%d.%d.%d.%d", ipv4[0], ipv4[1], ipv4[2], ipv4[3]);
		}
		printf("\n");
		printf("\tIPv6:");
		for (auto& ipv6 : it.second->IPv6Addr)
		{
			printf("\t%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
				ipv6[0], ipv6[1], ipv6[2], ipv6[3], ipv6[4], ipv6[5], ipv6[6], ipv6[7],
				ipv6[8], ipv6[9], ipv6[10], ipv6[11], ipv6[12], ipv6[13], ipv6[14], ipv6[15]);
		}
		printf("\n");
		printf("\tMAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			it.second->MACAddr[0], it.second->MACAddr[1], it.second->MACAddr[2],
			it.second->MACAddr[3], it.second->MACAddr[4], it.second->MACAddr[5]);
		printf("\tupload rate: %f\n", it.second->uploadRate);
		printf("\tdownload rate: %f\n", it.second->downloadRate);
		printf("\n");
	}

	freeifaddrs(head);
	close(socketFd);
}