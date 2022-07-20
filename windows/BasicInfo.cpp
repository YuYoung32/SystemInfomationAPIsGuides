//for basic use
#include <Windows.h>

// for CPU used ratio
#include <pdh.h>        
#pragma comment(lib,"pdh.lib")

#include <string>

void PrintComputerName();
void PrintComputerFullName();
void PrintUserName();
void PrintOSVersion();
void PrintCPUUsedRatio();
void PrintMemoryInfo();
void PrintSystemTime();
void PrintSystemRunningTime();


void PrintComputerName()
{
	WCHAR* name = nullptr;
	DWORD realLen = 0;
	// get suitable space
	GetComputerName(nullptr, &realLen);
	name = new(WCHAR[realLen]);
	GetComputerName(name, &realLen);
	wprintf(L"computer name: %s \n", name);
}

void PrintComputerFullName()
{
	WCHAR* name = nullptr;
	DWORD realLen = 0;
	// get suitable space
	GetComputerNameEx(ComputerNameDnsFullyQualified, nullptr, &realLen);
	name = new(WCHAR[realLen]);
	GetComputerNameEx(ComputerNameDnsFullyQualified, name, &realLen);
	wprintf(L"computer full name: %s \n", name);
}

void PrintUserName()
{
	WCHAR* name = nullptr;
	DWORD realLen = 0;
	// get suitable space
	GetUserName(nullptr, &realLen);
	name = new(WCHAR[realLen]);
	GetUserName(name, &realLen);
	wprintf(L"computer full name: %s \n", name);
}

void PrintOSVersion()
{
	typedef LONG(__stdcall* fnRtlGetVersion)(PRTL_OSVERSIONINFOEXW lpVersionInformation);
	RTL_OSVERSIONINFOEXW VersionInformation = { 0 };
	HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
	fnRtlGetVersion pRtlGetVersion = (fnRtlGetVersion)GetProcAddress(hNtdll, "RtlGetVersion");
	VersionInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
	LONG success = pRtlGetVersion(&VersionInformation);
	if (success != 0)
	{
		return;
	}

	ULONG dwMajorVersion = VersionInformation.dwMajorVersion;
	ULONG dwMinorVersion = VersionInformation.dwMinorVersion;
	UCHAR wProductType = VersionInformation.wProductType;
	DWORD buildNumber = VersionInformation.dwBuildNumber;

	std::string version = "Unknown";
	if (dwMajorVersion == 10)
	{
		if (wProductType == VER_NT_WORKSTATION)
		{
			version = "Windows10";
		}
		else
		{
			// build number
			// https://techcommunity.microsoft.com/t5/windows-server-insiders/windows-server-2019-version-info/m-p/291878/highlight/true#M856
			if (buildNumber == 17763)
			{
				version = "WindowsServer2016";
			}
			else if (buildNumber == 14393)
			{
				version = "WindowsServer2016";
			}
		}
	}
	else if (dwMajorVersion == 6)
	{
		if (wProductType == VER_NT_WORKSTATION)
		{
			if (dwMinorVersion == 3)
			{
				version = "Windows8dot1";
			}
			else if (dwMinorVersion == 2)
			{
				version = "Windows8";
			}
			else if (dwMinorVersion == 1)
			{
				version = "Windows7";
			}
			else if (dwMinorVersion == 0)
			{
				version = "WindowsVista";
			}
		}
		else
		{
			version = "WindowsServer2012";
		}
	}
	else if (dwMajorVersion == 5)
	{
		if (dwMinorVersion == 1 || dwMinorVersion == 2)
		{
			version = "WindowsXP";
		}
		else if (dwMinorVersion == 0)
		{
			version = "Windows2000";
		}
	}
	printf("OS version: %s \n", version.c_str());
}

void PrintCPUUsedRatio()
{
	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuCounter;
	PDH_FMT_COUNTERVALUE cpuCounterVal;

	PdhOpenQuery(NULL, NULL, &cpuQuery);
	// counter path
	LPCWSTR path = L"\\Processor Information(_Total)\\% Processor Utility";
	PdhAddEnglishCounter(cpuQuery, path, NULL, &cpuCounter);
	PdhCollectQueryData(cpuQuery);
	// get past 1000ms data
	Sleep(1000);
	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuCounter, PDH_FMT_DOUBLE, NULL, &cpuCounterVal);
	float ratio = (float)cpuCounterVal.doubleValue;

	printf("now cpu used ratio: %f \n", ratio);
}

void PrintMemoryInfo()
{
	MEMORYSTATUSEX st;
	st.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&st);
	float totalMemory = (float)(st.ullTotalPhys) / (1024 * 1024 * 1024);
	float usedMemory = (float)(st.ullTotalPhys - st.ullAvailPhys) / (1024 * 1024 * 1024);
	printf("total memory(GB) %f \n", totalMemory);
	printf("used memory(GB) %f \n", usedMemory);
}

void PrintSystemTime()
{
	SYSTEMTIME stSystemTime;
	GetSystemTime(&stSystemTime);
	printf("system now time: %d-%d-%d-%d-%d \n",
		stSystemTime.wYear,
		stSystemTime.wMonth,
		stSystemTime.wDay,
		stSystemTime.wHour,
		stSystemTime.wMinute
	);
}

void PrintSystemRunningTime()
{
	ULONGLONG duration = GetTickCount64();
	printf("system has running(second unit): %lld\n", duration / 1000);
}
