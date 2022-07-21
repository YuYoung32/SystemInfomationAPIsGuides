// for computer name, full name, OS version
#include <sys/utsname.h>

// for user name
#include <pwd.h>
#include <sys/unistd.h>

// for memory infomation
#include <sys/sysinfo.h>

// for time
#include <time.h>

// for sleep
#include <unistd.h>

#include <string>
#include <iostream>

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
	struct utsname buffer;
	uname(&buffer);
	char* name = buffer.nodename;
	printf("computer name: %s\n", name);
}

void PrintComputerFullName()
{
	struct utsname buffer;
	int success = uname(&buffer);
	std::string name;
	if (std::string(buffer.domainname) == "(none)")
	{
		name = std::string(buffer.nodename);
	}
	else
	{
		name = std::string(buffer.nodename) + "." + std::string(buffer.domainname);
	}
	printf("computer full name: %s\n", name.c_str());
}

void PrintUserName()
{
	struct passwd* pwd = getpwuid(getuid());
	char* name = pwd->pw_name;
	printf("login user name: %s\n", name);
}

void PrintOSVersion()
{
	struct utsname buffer;
	int success = uname(&buffer);
	char* version = buffer.release;
	printf("OS version: %s\n", version);

}

void PrintCPUUsedRatio()
{
	struct CPUInfo
	{
		/*
			时间单位都为0.01s
			The total time is the sum of the following seven items: user+nice+kernel+idle+iowait+irq+softirq
			CPU idle time: idle
		*/
		unsigned int user;    // process(nice>=0) running in user mode time
		unsigned int nice;    // process(nice<0) running in user mode time
		unsigned int kernel;  // process(nice<0) running in kernel mode time
		unsigned int idle;    // wait time except for disk IO time
		unsigned int iowait;  // disk IO time
		unsigned int irq;     // hard interruptions
		unsigned int softirq; // soft interruptions
	};
	auto ReadCPUData = [&](struct CPUInfo& cpuInfo)
	{
		FILE* fd = fopen("/proc/stat", "r");
		if (fd == NULL)
		{
			return;
		}

		char buff[256];
		char name[32];
		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %u %u %u %u %u %u %u",
			name,
			&cpuInfo.user,
			&cpuInfo.nice,
			&cpuInfo.kernel,
			&cpuInfo.idle,
			&cpuInfo.iowait,
			&cpuInfo.irq,
			&cpuInfo.softirq);
		fclose(fd);
	};

	auto CaculateRatio = [&](struct CPUInfo pre, struct CPUInfo cur) -> float
	{
		unsigned int preTotal, curTotal;
		preTotal = pre.user + pre.nice + pre.kernel + pre.idle + pre.softirq + pre.iowait + pre.irq;
		curTotal = cur.user + cur.nice + cur.kernel + cur.idle + cur.softirq + cur.iowait + cur.irq;
		return 100.0 - ((float)(cur.idle - pre.idle)) / (curTotal - preTotal) * 100.0;
	};

	struct CPUInfo pre;
	struct CPUInfo cur;
	ReadCPUData(pre);
	sleep(1);
	ReadCPUData(cur);

	float ratio = CaculateRatio(pre, cur);
	printf("now cpu used ratio: %f\n", ratio);
}

void PrintMemoryInfo()
{
	struct sysinfo info;
	sysinfo(&info);

	float totalMemory = (float)info.totalram / 1024 / 1024 / 1024;
	float usedMemory = (float)(info.totalram - info.freeram) / 1024 / 1024 / 1024;
	printf("total memory(GB) %f \n", totalMemory);
	printf("used memory(GB) %f \n", usedMemory);
}

void PrintSystemTime()
{
	time_t tNow;
	struct tm tmNow;
	time(&tNow);
	gmtime_r(&tNow, &tmNow);
	printf("system now time: %d-%d-%d-%d-%d-%d \n",
		tmNow.tm_year + 1900,
		tmNow.tm_mon + 1,
		tmNow.tm_mday,
		tmNow.tm_hour + 8,
		tmNow.tm_min,
		tmNow.tm_sec);
}

void PrintSystemRunningTime()
{
	struct sysinfo info;
	sysinfo(&info);
	long duration = info.uptime;
	printf("system has running(second unit): %ld\n", duration);
}
