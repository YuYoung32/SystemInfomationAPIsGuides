# SystemInfomationAPIsGuides

[English documentation](./README_en.md)

旨在为了简化开发人员需要获取系统信息时的搜索过程。本仓库提供了常见的信息如系统名称、网络适配器地址、磁盘容量的API小Demo，并叙述了写这些API时容易进入的误区。提供的Demo通常包括Windows平台（在Windows10上测试）和Linux平台（在kernel版本5+上测试）。能够获取的信息如下目录。欢迎各位系统软件开发者提出需求或补充Demo。

[TOC]

## Windows

### 系统基本信息

#### 电脑名称与全称

**显示结果：**

```text
DESKTOP-ABCD
aa.bb.DESKTOP-ABCD
```

**API文档：**

[GetComputerNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcomputernamea)

[GetComputerNameExA function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getcomputernameexa)



#### 当前系统登陆的用户名

**显示结果：**

```
Alice
```

**API文档：**

[GetUserNameA function (winbase.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamea)



#### 系统版本

**显示结果：**

```
Windows10
```

**注意：**

版本信息在`VersionHelpers.h`里通过类似`IsWindows10OrGreater()`和`IsWindowsServer()`来获取，但是这个API目的是为了测试软件的兼容性而不是获取版本，Windows8及以上依赖于`manifest list`，若没有此文件，此API只会得到系统版本是Windows8的结果。

> ***** For applications that have been manifested for Windows 8.1 or Windows 10. Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS version value (6.2). To manifest your applications for Windows 8.1 or Windows 10, refer to [Targeting your application for Windows](https://docs.microsoft.com/en-us/windows/win32/sysinfo/targeting-your-application-at-windows-8-1).

因此如要得到更加详细的信息需要通过`RtlGetVersion`动态获取。

[Operating System Version - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/sysinfo/operating-system-version)



#### CPU利用率

**显示结果：**

```text
50.0
```

**注意：**

一种常规的思路是根据CPU运行的空闲时间和忙碌时间来算出使用率，在Linux上确实是这么算的。但是Windows上若要和任务管理器里显示的值对比，发现此计算的值会小一些。原因是“利用率”的定义问题，任务管理器中利用率实际上是结合了CPU当前频率算出来的，而不是简单的根据CPU时间计算。幸运的是我们不需要知道算法，在性能监视器里可以直接拿到这个值。

我们可以在性能监视器里看到，一个是`Processor Time`，还有一个是`Processor Utility`，`Processor Time`是根据CPU运行的空闲时间和忙碌时间来算出来的，而`Processor Utility`才是任务管理器里显示的数值。通过性能计数器，我们可以拿到此数值。

[Using Performance Counters - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/perfctrs/using-performance-counters)



#### 内存使用情况

**显示结果：**

```text
7.8GB
```

**API文档：**

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)



#### 系统时间和运行时间

**显示结果：**

```text
2022-1-31-15-30
10932s
```

**API文档：**

[GlobalMemoryStatusEx function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)

[GetTickCount64 function (sysinfoapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount64)



### 网络适配器信息

#### 网卡IP地址、MAC地址、状态

**显示结果：**

```text
192.168.1.30
1f:2d:3c:4d:5f:6e
OK
```

**注意：**

此API会返回一个链表，该链表会列出所有网络适配器包括虚拟网络适配器（如虚拟机、回环地址）的信息，包括IP地址和MAC地址，IP地址可能会有多个，应注意遍历全部。此外，IPv4和IPv6的传入FLAG不同，返回的是不同的链表。

[GetAdaptersAddresses function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses)



#### 网卡上传下载速率

**显示结果：**

```text
16.0KB/s
```

**注意：**

`GetAdaptersAddresses`API返回的链表的节点中有`Speed`字段，但此字段是指网卡的最大速率。要想获得传输速率需要读取电脑全部的物理接口，获得累积的流量信息，自行计算。此`GetIfTable`API会返回所有物理接口，因此需要根据MAC地址筛选网卡的接口。

[GetIfTable function (iphlpapi.h) - Win32 apps | Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getiftable)

