# SystemInfomationAPIsGuides

languages: [English documentation](./README_en.md)

旨在为了简化开发人员需要获取系统信息时的搜索过程。本仓库提供了常见的信息如系统名称、网络适配器地址、磁盘容量的API小Demo，并叙述了写这些API时容易进入的误区。提供的Demo通常包括**Windows平台**（在Windows10上测试）和**Linux平台**（在kernel版本5+上测试）。能够获取的信息如下目录。欢迎各位系统软件开发者提出需求或补充Demo。

---



- [SystemInfomationAPIsGuides](#systeminfomationapisguides)
  - [Windows](#windows)
    - [系统基本信息](#%E7%B3%BB%E7%BB%9F%E5%9F%BA%E6%9C%AC%E4%BF%A1%E6%81%AF)
      - [电脑名称与全称](#%E7%94%B5%E8%84%91%E5%90%8D%E7%A7%B0%E4%B8%8E%E5%85%A8%E7%A7%B0)
      - [当前系统登陆的用户名](#%E5%BD%93%E5%89%8D%E7%B3%BB%E7%BB%9F%E7%99%BB%E9%99%86%E7%9A%84%E7%94%A8%E6%88%B7%E5%90%8D)
      - [系统版本](#%E7%B3%BB%E7%BB%9F%E7%89%88%E6%9C%AC)
      - [CPU利用率](#cpu%E5%88%A9%E7%94%A8%E7%8E%87)
      - [内存使用情况](#%E5%86%85%E5%AD%98%E4%BD%BF%E7%94%A8%E6%83%85%E5%86%B5)
      - [系统时间和运行时间](#%E7%B3%BB%E7%BB%9F%E6%97%B6%E9%97%B4%E5%92%8C%E8%BF%90%E8%A1%8C%E6%97%B6%E9%97%B4)
    - [网络适配器信息](#%E7%BD%91%E7%BB%9C%E9%80%82%E9%85%8D%E5%99%A8%E4%BF%A1%E6%81%AF)
      - [网卡IP地址、MAC地址、状态](#%E7%BD%91%E5%8D%A1ip%E5%9C%B0%E5%9D%80mac%E5%9C%B0%E5%9D%80%E7%8A%B6%E6%80%81)
      - [网卡上传下载速率](#%E7%BD%91%E5%8D%A1%E4%B8%8A%E4%BC%A0%E4%B8%8B%E8%BD%BD%E9%80%9F%E7%8E%87)
  - [Linux](#linux)
    - [前言](#%E5%89%8D%E8%A8%80)
    - [系统基本信息](#%E7%B3%BB%E7%BB%9F%E5%9F%BA%E6%9C%AC%E4%BF%A1%E6%81%AF-1)
      - [电脑名称与全称](#%E7%94%B5%E8%84%91%E5%90%8D%E7%A7%B0%E4%B8%8E%E5%85%A8%E7%A7%B0-1)
      - [当前系统登陆的用户名](#%E5%BD%93%E5%89%8D%E7%B3%BB%E7%BB%9F%E7%99%BB%E9%99%86%E7%9A%84%E7%94%A8%E6%88%B7%E5%90%8D-1)
      - [系统版本](#%E7%B3%BB%E7%BB%9F%E7%89%88%E6%9C%AC-1)
      - [CPU利用率](#cpu%E5%88%A9%E7%94%A8%E7%8E%87-1)
      - [内存使用情况](#%E5%86%85%E5%AD%98%E4%BD%BF%E7%94%A8%E6%83%85%E5%86%B5-1)
      - [系统时间和运行时间](#%E7%B3%BB%E7%BB%9F%E6%97%B6%E9%97%B4%E5%92%8C%E8%BF%90%E8%A1%8C%E6%97%B6%E9%97%B4-1)
    - [网络适配器信息](#%E7%BD%91%E7%BB%9C%E9%80%82%E9%85%8D%E5%99%A8%E4%BF%A1%E6%81%AF-1)
      - [网卡名称、网卡IPv4地址、IPv6地址、状态、网卡上传下载速率](#%E7%BD%91%E5%8D%A1%E5%90%8D%E7%A7%B0%E7%BD%91%E5%8D%A1ipv4%E5%9C%B0%E5%9D%80ipv6%E5%9C%B0%E5%9D%80%E7%8A%B6%E6%80%81%E7%BD%91%E5%8D%A1%E4%B8%8A%E4%BC%A0%E4%B8%8B%E8%BD%BD%E9%80%9F%E7%8E%87)
      - [MAC地址](#mac%E5%9C%B0%E5%9D%80)

---



## Windows

### 系统基本信息

#### 电脑名称与全称

显示结果：

```text
computer name: DESKTOP-ABCD
computer full name: aa.bb.DESKTOP-ABCD
```

API文档：

[GetComputerNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcomputernamea)

[GetComputerNameExA function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getcomputernameexa)





#### 当前系统登陆的用户名

显示结果：

```
login user name: Alice
```

API文档：

[GetUserNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamea)





#### 系统版本

显示结果：

```
OS version: Windows10
```

**注意：**

版本信息在`VersionHelpers.h`里通过类似`IsWindows10OrGreater()`和`IsWindowsServer()`来获取，但是这个API目的是为了测试软件的兼容性而不是获取版本，Windows8及以上依赖于`manifest list`，若没有此文件，此API只会得到系统版本是Windows8的结果。

> ***** For applications that have been manifested for Windows 8.1 or Windows 10. Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS version value (6.2). To manifest your applications for Windows 8.1 or Windows 10, refer to [Targeting your application for Windows](https://docs.microsoft.com/en-us/windows/win32/sysinfo/targeting-your-application-at-windows-8-1).

因此如要得到更加详细的信息需要通过`RtlGetVersion`动态获取。

[Operating System Version - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/sysinfo/operating-system-version)





#### CPU利用率

显示结果：

```text
CPU utilization: 50.0
```

**注意：**

一种常规的思路是根据CPU运行的空闲时间和忙碌时间来算出使用率，在Linux上确实是这么算的。但是Windows上若要和任务管理器里显示的值对比，发现此计算的值会小一些。原因是“利用率”的定义问题，任务管理器中利用率实际上是结合了CPU当前频率算出来的，而不是简单的根据CPU时间计算。幸运的是我们不需要知道算法，在性能监视器里可以直接拿到这个值。

我们可以在性能监视器里看到，一个是`Processor Time`，还有一个是`Processor Utility`，`Processor Time`是根据CPU运行的空闲时间和忙碌时间来算出来的，而`Processor Utility`才是任务管理器里显示的数值。通过性能计数器，我们可以拿到此数值。

[Using Performance Counters - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/perfctrs/using-performance-counters)





#### 内存使用情况

显示结果：

```text
memory total size: 7.8GB
memory used size: 5.8GB
```

API文档：

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)





#### 系统时间和运行时间

显示结果：

```text
system now time: 2022-1-31-15-30
system has run: 10932s
```

API文档：

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)

[GetTickCount64 function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount64)





### 网络适配器信息

#### 网卡IP地址、MAC地址、状态

显示结果：

```text
IPv4 address: 192.168.1.30
MAC address: 1f:2d:3c:4d:5f:6e
status: OK
```

**注意：**

此API会返回一个链表，该链表会列出所有网络适配器包括虚拟网络适配器（如虚拟机、回环地址）的信息，包括IP地址和MAC地址，IP地址可能会有多个，应注意遍历全部。此外，IPv4和IPv6的传入FLAG不同，返回的是不同的链表。

[GetAdaptersAddresses function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses)





#### 网卡上传下载速率

显示结果：

```text
upload rate: 10.1KB/
download rate: 16.0KB/s
```

**注意：**

`GetAdaptersAddresses`API返回的链表的节点中有`Speed`字段，但此字段是指网卡的最大速率。要想获得传输速率需要读取电脑全部的物理接口，获得累积的流量信息，自行计算。此`GetIfTable`API会返回所有物理接口，因此需要根据MAC地址筛选网卡的接口。

[GetIfTable function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getiftable)





## Linux

### 前言

Linux系统不像Windows那样有便捷的、统一的API可以直接拿到数据，大多数也没有很完美的实现案例。这里列举出几种实现思路。

1. **使用shell命令**

   shell命令嵌入到程序中执行，然后保存结果。这种方法的优点是很简单，系统的shell命令已经帮我们做了海量的处理。缺点是只能拿到字符数据，需要自己解析，另外使用这种方法实质上是在后台新建进程通过`bin/sh`运行命令然后拿到输出字符。这种方法涉及到进程的上下文切换开销较大，与直接调用具体功能的API有百倍速度差距。若不考虑性能问题，使用这种方法最为方便。

   具体API如下

   [popen(3) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/popen.3.html)

2. **读取`/proc`文件系统里的文件**

   `/proc`是一个在RAM里的临时的文件系统，保存着系统运行的信息。我们可以通过读取文件然后拿到数据，这种方法虽然也伴随着文件的读写，会产生IO中断，但是比方法1相比性能消耗会小一些。

3. **使用用户API**

   这种方法类似于Windows获取信息的方法，利用系统提供的用户API获取到较为原始的信息。直接调用API性能消耗会比较小。

4. **使用内核API**

   这种方法最接近底层，性能消耗最小，但是编程复杂度大幅度提升。

总的来说，从性能优秀程度上来看方法1<2<3<4，从获取信息的方便程度上1>2>3>4。本手册主要使用方法2和方法3，即读取`/proc`文件系统和用户API。

### 系统基本信息

#### 电脑名称与全称

显示结果：

```text
computer name: Ubuntu
computer full name: Unbuntu.aa.bb
```

API文档：

[uname(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/uname.2.html)





#### 当前系统登陆的用户名

显示结果：

```
login user name: Alice
```

API文档：

[getuid(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/geteuid.2.html)

[getpwuid(3p) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/getpwuid.3p.html)





#### 系统版本

显示结果：

```
OS version: Ubuntu
```

**注意：**

这里获取到的是内核版本。

[[uname(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/uname.2.html)](https://docs.microsoft.com/en-us/windows/win32/sysinfo/operating-system-version)





#### CPU利用率

显示结果：

```text
CPU utilization: 50.0
```

**注意：**

根据CPU运行的空闲时间和忙碌时间来算出使用率，这里需要从`/proc`文件系统里读取CPU统计数据进行计算。

文件路径为：/proc/stat





#### 内存使用情况

显示结果：

```text
memory total size: 7.8GB
memory used size: 7.8GB=
```

API文档：

[sysinfo(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/sysinfo.2.html)





#### 系统时间和运行时间

显示结果：

```text
system now time: 2022-1-31-15-30
system has run: 10932s
```

API文档：

[time(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/time.2.html)

[gmtime(3p) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/gmtime.3p.html)

[sysinfo(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/sysinfo.2.html)





### 网络适配器信息

#### 网卡名称、网卡IPv4地址、IPv6地址、状态、网卡上传下载速率

显示结果：

```text
NIC name: eth0
status: OK
IPv4 address: 192.168.1.30
IPv6 address: 2001:db8:85a3:8d3:1319:8a2e:370:7348
upload rate: 10.1KB/s
download rate: 16.0KB/s
```

**注意：**

此API会返回一个链表，该链表会列出所有网络适配器包括虚拟网络适配器（如虚拟机、回环地址）的信息，包括名称、IPv4地址、IPv6地址、传输字节数。但是这里每个网卡的每一项都是单独的一个链表节点，同一个网卡的链表节点具有相同的名字，但根据FLAG的不同而存储不同项。因此需要遍历全部才能收集齐一个网卡的信息。

[getifaddrs(3) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/getifaddrs.3.html)





#### MAC地址

显示结果：

```text
MAC address: 1f:2d:3c:4d:5f:6e
```

**注意：**

使用`ioctl`来获取接口的硬件地址，需要实现创建socket，通过该socket和网卡名称来获取MAC地址。

[ioctl(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/ioctl.2.html)

