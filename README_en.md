# SystemInfomationAPIsGuides

[中文文档](./README_zh.md)

It is designed to simplify the search process when developers need to get system information. This repository provides small API demos for common information such as system name, network adapter address, and disk capacity, and describes the misconceptions that can easily be entered when writing these APIs. The demos provided usually include Windows platform (tested on Windows 10) and Linux platform (tested on kernel version 5+). The information that can be obtained is listed below. All system-software developers are welcome to submit requests or add demos.

[TOC]

## Windows

### System Basic Information

#### Computer name and full name

**Display results:**

```text
DESKTOP-ABCD
aa.bb.DESKTOP-ABCD
```

**API documentation:**

[GetComputerNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcomputernamea)

[GetComputerNameExA function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getcomputernameexa)



#### The current system login username

**Display results:**

```
Alice
```

**API documentation:**

[GetUserNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamea)



#### OS version

**Display results:**

```
Windows10
```

**Notes:**

Version information is obtained in `VersionHelpers.h` by similar `IsWindows10OrGreater()` and `IsWindowsServer()`, but the purpose of this API is to test the compatibility of the software rather than to obtain the version, Windows8 and above depend on the manifest list. If this file is not exist, this API will only get the result that the system version is Windows 8.

> ***** For applications that have been manifested for Windows 8.1 or Windows 10. Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS version value (6.2). To manifest your applications for Windows 8.1 or Windows 10, refer to [Targeting your application for Windows](https://docs.microsoft.com/en-us/windows/win32/sysinfo/targeting-your-application-at-windows-8-1).

So if we want to get more detailed information, you need to get it dynamically through `RtlGetVersion`.

[Operating System Version - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/sysinfo/operating-system-version)



#### CPU utilization

**Display results:**

```text
50.0
```

**Notes:**

One conventional idea is to calculate the usage based on the idle time and busy time when the CPU running, which is indeed the case on Linux. But on Windows, if you compare it with the value shown in Task Manager, you will find that the calculated value is smaller. The reason for this is the definition of "utilization", which in Task Manager is actually calculated by combining the current CPU frequency, not simply by CPU time. Fortunately, we don't need to know the algorithm, we can get this value directly in the `Performance Counter`.

We can see in the Performance Monitor, one is `Processor Time` and another is `Processor Utility`. `Processor Time` is calculated based on the idle time and busy time, while `Processor Utility` is the value displayed in the Task Manager. By using the `Performance Counter`, we can get this value.

[Using Performance Counters - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/perfctrs/using-performance-counters)



#### Memory use

**Display results:**

```text
7.8GB
```

**API documentation:**

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)



#### System time and running time

**Display results:**

```text
2022-1-31-15-30
10932s
```

**API documentation:**

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)

[GetTickCount64 function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount64)



### Network adapter information

#### NIC IP address, MAC address, status

**Display results:**

```text
192.168.1.30
1f:2d:3c:4d:5f:6e
OK
```

**Notes:**

This API returns a chain list that lists information about all network adapters including virtual network adapters (e.g. virtual machines, loopback addresses), including IP addresses and MAC addresses, there may be more than one IP address and care should be taken to traverse them all. In addition, IPv4 and IPv6 have different incoming FLAGs and return different chained tables.

[GetAdaptersAddresses function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses)



#### NIC upload and download rates

**Display results:**

```text
16.0KB/s
```

**Notes:**

The node of the chain list returned by `GetAdaptersAddresses` API has a Speed field, but this field refers to the maximum rate of the network card. To get the transfer rate you need to read all the physical interfaces of your computer, get the accumulated traffic information and calculate it by yourself. This 	`GetIfTable`  API returns all physical interfaces, so you need to filter the interface of the NIC according to the MAC address.

[GetIfTable function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getiftable)