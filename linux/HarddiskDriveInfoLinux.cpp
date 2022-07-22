// for statfs
#include <sys/vfs.h>

#include <stdio.h>
#include <string>

void PrintDriveNamesAndCaps();

void PrintDriveNamesAndCaps()
{
	// this file record info about disk mounted
	const char* filePath = "/proc/mounts";
	FILE* fd = fopen(filePath, "r");
	constexpr auto BUFF_SIZE = 1024;
	char buff[1024];

	printf("hard disk:\n");
	while (fgets(buff, BUFF_SIZE, fd) != NULL)
	{
		// the disk info is like "/dev/sdx mount_path ..."
		if (std::string(buff, buff + 7) != "/dev/sd")
		{
			continue;
		}
		int pos = 0;
		std::string diskName, mountPath;
		while (buff[pos] != ' ')
		{
			diskName.push_back(buff[pos]);
			pos++;
		}
		pos++;
		printf("\tdisk name: %s, ", diskName.c_str());

		while (buff[pos] != ' ')
		{
			mountPath.push_back(buff[pos]);
			pos++;
		}
		printf("mount path: %s, ", mountPath.c_str());

		struct statfs diskInfo;
		statfs(mountPath.c_str(), &diskInfo);
		printf("total space: %.2fGB, free space: %.2fGB\n",
			(float)diskInfo.f_blocks * diskInfo.f_bsize / 1024 / 1024 / 1024,
			(float)diskInfo.f_bavail * diskInfo.f_bsize / 1024 / 1024 / 1024);
	}
}