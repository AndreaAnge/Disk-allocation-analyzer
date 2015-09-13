#include "StdAfx.h"
#include "DiskSpaceInfo.h"
#include <Windows.h>


DiskSpaceInfo::DiskSpaceInfo(void)
{
}


DiskSpaceInfo::~DiskSpaceInfo(void)
{
}

void InitializeMembers(void)
{
	ZeroMemory(&Capacity, sizeof(ULARGE_INTEGER));
	ZeroMemory(&Used, sizeof(ULARGE_INTEGER));
	ZeroMemory(&Free, sizeof(ULARGE_INTEGER));
}

void GetDiskSpace(CString driveLetter,ULARGE_INTEGER &Capacity, 
					ULARGE_INTEGER &Used, 
					ULARGE_INTEGER &Free)
{
	ULARGE_INTEGER AvailableToCaller;
	ZeroMemory(&AvailableToCaller, sizeof(ULARGE_INTEGER));
	
	InitializeMembers();

	TRY
	{
		GetDiskFreeSpaceEx(driveLetter, &AvailableToCaller,
									&Capacity, &Free);
		Used.QuadPart = Capacity.QuadPart - Free.QuadPart;
	}
	END_TRY	
}

