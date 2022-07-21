# SystemInfomationAPIsGuides

languages: [中文文档](./README.md)

It is designed to simplify the search process when developers need to get system information. This repository provides small API demos for common information such as system name, network adapter address, and disk capacity, and describes the misconceptions that can easily be entered when writing these APIs. The demos provided usually include Windows platform (tested on Windows 10) and Linux platform (tested on kernel version 5+). The information that can be obtained is listed below. All system-software developers are welcome to submit requests or add demos.

---

**CONTENTS**

- [SystemInfomationAPIsGuides](#systeminfomationapisguides)
  - [Windows](#windows)
    - [System Basic Information](#system-basic-information)
      - [Computer name and full name](#computer-name-and-full-name)
      - [The current system login username](#the-current-system-login-username)
      - [OS version](#os-version)
      - [CPU utilization](#cpu-utilization)
      - [Memory use](#memory-use)
      - [System time and running time](#system-time-and-running-time)
    - [Network adapter information](#network-adapter-information)
      - [NIC IP address, MAC address, status](#nic-ip-address-mac-address-status)
      - [NIC upload and download rates](#nic-upload-and-download-rates)
  - [Linux](#linux)
    - [Preface](#preface)
    - [System Basic Information](#system-basic-information-1)
      - [Computer name and full name](#computer-name-and-full-name-1)
      - [The current system login username](#the-current-system-login-username-1)
      - [OS version](#os-version-1)
      - [CPU utilization](#cpu-utilization-1)
      - [Memory use](#memory-use-1)
      - [System time and running time](#system-time-and-running-time-1)
    - [Network adapter information](#network-adapter-information-1)
      - [NIC IPv4 address, IPv6 address, status, upload and download rates](#nic-ipv4-address-ipv6-address-status-upload-and-download-rates)
      - [NIC MAC address](#nic-mac-address)

---



## Windows

### System Basic Information

#### Computer name and full name

**Display results:**

```text
computer name: DESKTOP-ABCD
computer full name: aa.bb.DESKTOP-ABCD
```

**API documentation:**

[GetComputerNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcomputernamea)

[GetComputerNameExA function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getcomputernameexa)





#### The current system login username

**Display results:**

```
login user name: Alice
```

**API documentation:**

[GetUserNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamea)





#### OS version

**Display results:**

```
OS version: Windows10
```

**Notes:**

Version information is obtained in `VersionHelpers.h` by similar `IsWindows10OrGreater()` and `IsWindowsServer()`, but the purpose of this API is to test the compatibility of the software rather than to obtain the version, Windows8 and above depend on the manifest list. If this file is not exist, this API will only get the result that the system version is Windows 8.

> ***** For applications that have been manifested for Windows 8.1 or Windows 10. Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS version value (6.2). To manifest your applications for Windows 8.1 or Windows 10, refer to [Targeting your application for Windows](https://docs.microsoft.com/en-us/windows/win32/sysinfo/targeting-your-application-at-windows-8-1).

So if we want to get more detailed information, you need to get it dynamically through `RtlGetVersion`.

[Operating System Version - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/sysinfo/operating-system-version)





#### CPU utilization

**Display results:**

```text
CPU utilization: 50.0
```

**Notes:**

One conventional idea is to calculate the usage based on the idle time and busy time when the CPU running, which is indeed the case on Linux. But on Windows, if you compare it with the value shown in Task Manager, you will find that the calculated value is smaller. The reason for this is the definition of "utilization", which in Task Manager is actually calculated by combining the current CPU frequency, not simply by CPU time. Fortunately, we don't need to know the algorithm, we can get this value directly in the `Performance Counter`.

We can see in the Performance Monitor, one is `Processor Time` and another is `Processor Utility`. `Processor Time` is calculated based on the idle time and busy time, while `Processor Utility` is the value displayed in the Task Manager. By using the `Performance Counter`, we can get this value.

[Using Performance Counters - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/perfctrs/using-performance-counters)





#### Memory use

**Display results:**

```text
memory total size: 7.8GB
memory used size: 5.8GB
```

**API documentation:**

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)





#### System time and running time

**Display results:**

```text
system now time: 2022-1-31-15-30
system has run: 10932s
```

**API documentation:**

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)

[GetTickCount64 function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount64)





### Network adapter information

#### NIC IP address, MAC address, status

**Display results:**

```text
IPv4 address: 192.168.1.30
MAC address: 1f:2d:3c:4d:5f:6e
status: OK
```

**Notes:**

This API returns a chain list that lists information about all network adapters including virtual network adapters (e.g. virtual machines, loopback addresses), including IP addresses and MAC addresses, there may be more than one IP address and care should be taken to traverse them all. In addition, IPv4 and IPv6 have different incoming FLAGs and return different chained tables.

[GetAdaptersAddresses function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses)





#### NIC upload and download rates

**Display results:**

```text
upload rate: 10.1KB/
download rate: 16.0KB/s
```

**Notes:**

The node of the chain list returned by `GetAdaptersAddresses` API has a Speed field, but this field refers to the maximum rate of the network card. To get the transfer rate you need to read all the physical interfaces of your computer, get the accumulated traffic information and calculate it by yourself. This 	`GetIfTable`  API returns all physical interfaces, so you need to filter the interface of the NIC according to the MAC address.

[GetIfTable function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getiftable)

---

## Linux

### Preface

Linux systems do not have a convenient and unified API to get the data directly like Windows, and most of them do not have perfect implementation cases. Here is a list of several implementation ideas.

1. **Using shell commands**

   The shell command is embedded in the program and executed, and then the result is transfered to the program. The advantage of this method is that it is very simple and the system shell commands have already done the bulk of the machining. The disadvantage is that we can only get the character data and need to parse it by ourselves. In addition, using this method is essentially creating a new process in the background to run the command through `bin/sh` and then get the output characters. This method involves a large overhead of process context switching, and a hundred times slower than directly calling the API for a specific purpose. If performance is not a concern, it is most convenient to use this method.

   The specific APIs are as follows.

   [popen(3) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/popen.3.html)

2. **Read files from the `/proc` file system**

   `/proc` is a temporary file system in RAM that holds information about the system operation. We can read the file and then get the data. Although this method is also accompanied by file reading and writing and will generate IO interrupts, it will use less performance than method 1.

3. **Using the User API**

   This approach is similar to the way Windows gets information, using the user API provided by the system to get more primitive information. Direct API calls are less performance intensive.

4. **Using the Kernel API**

   This approach is closest to the bottom and has the lowest performance consumption, but the programming complexity is substantially higher.

In summary, method 1 < 2 < 3 < 4 in terms of performance excellence, and 1 > 2 > 3 > 4 in terms of ease of access to information. This guides mainly uses method 2 and method 3, i.e., reading the /proc file system and the user API.

### System Basic Information

#### Computer name and full name

**Display results:**

```text
computer name: Ubuntu
computer full name: Unbuntu.aa.bb
```

**API documentation：**

[uname(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/uname.2.html)





#### The current system login username

**Display results:**

```
login user name: Alice
```

**API documentation：**

[getuid(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/geteuid.2.html)

[getpwuid(3p) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/getpwuid.3p.html)





#### OS version

**Display results:**

```
OS version: Ubuntu
```

**Notes:**

The kernel version is obtained here.

[[uname(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/uname.2.html)](https://docs.microsoft.com/en-us/windows/win32/sysinfo/operating-system-version)





#### CPU utilization

**Display results:**

```text
CPU utilization: 50.0
```

**Notes:**

The utilization is calculated based on the idle time and busy time of CPU operation, where CPU statistics are read from the `/proc` file system for calculation.

The file path is: /proc/stat





#### Memory use

**Display results:**

```text
memory total size: 7.8GB
memory used size: 7.8GB=
```

**API documentation：**

[sysinfo(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/sysinfo.2.html)





#### System time and running time

**Display results:**

```text
system now time: 2022-1-31-15-30
system has run: 10932s
```

**API documentation：**

[time(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/time.2.html)

[gmtime(3p) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/gmtime.3p.html)

[sysinfo(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/sysinfo.2.html)





### Network adapter information

#### NIC IPv4 address, IPv6 address, status, upload and download rates

**Display results:**

```text
NIC name: eth0
status: OK
IPv4 address: 192.168.1.30
IPv6 address: 2001:db8:85a3:8d3:1319:8a2e:370:7348
upload rate: 10.1KB/s
download rate: 16.0KB/s
```

**Notes:**

This API returns a chain list that contains information about all network adapters including virtual network adapters (e.g. virtual machines, loopback addresses), including name, IPv4 address, IPv6 address, number of bytes transferred. But here each item of each NIC is a separate chain table node, and the chain list node of the same NIC has the same name, but stores different items depending on the FLAG. Therefore, it is required to traverse the list to collect all the information of a NIC.

[getifaddrs(3) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/getifaddrs.3.html)





#### NIC MAC address

**Display results:**

```text
MAC address: 1f:2d:3c:4d:5f:6e
```

**Notes:**

To use `ioctl` to get the hardware address of the interface, we need to implement the creation of a socket and get the MAC address by that socket and the name of the NIC.

[ioctl(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/ioctl.2.html)



