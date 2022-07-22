// for drive names
#include <Windows.h>

#include <stdio.h>

void PrintDriveNamesAndCaps();

void PrintDriveNamesAndCaps()
{

	WCHAR namesBuffer[256] = { '\0' };
	int cnt = 0;
	int charsAmount = GetLogicalDriveStrings(sizeof(namesBuffer) - 1, namesBuffer);
	// the name buffer returned is like "C://\0D://\0", in a other word, the drive name are splited by '\0'
	int pos = 0;
	printf("hard disk:\n");
	for (int i = 0; i < charsAmount; i++)
	{
		if (namesBuffer[i] == '\0')
		{
			ULARGE_INTEGER availToCallerSpace, total, freeSpace;
			WCHAR* diskRootPath = namesBuffer + pos;
			GetDiskFreeSpaceEx(diskRootPath, &availToCallerSpace, &total, &freeSpace);
			wprintf(L"\tdisk name: %s, total spcae: %.2fGB, free space: %.2fGB \n",
				diskRootPath,
				total.QuadPart / 1024.0 / 1024.0 / 1024.0,
				freeSpace.QuadPart / 1024.0 / 1024.0 / 1024.0);
			pos = i + 1;
		}
	}
}