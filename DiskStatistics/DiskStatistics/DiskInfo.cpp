// DiskInfo.cpp : implementation file
//

#include "stdafx.h"
#include "DiskStatistics.h"
#include "DiskInfo.h"


// DiskInfo

IMPLEMENT_DYNAMIC(DiskInfo, CStatic)

DiskInfo::DiskInfo()
{
	InitMembers();
}

DiskInfo::~DiskInfo()
{
}


BEGIN_MESSAGE_MAP(DiskInfo, CStatic)
END_MESSAGE_MAP()



// DiskInfo message handlers
void DiskInfo::DisplayDrive(CString driveLetter)
{
	
	pDiskLetter=driveLetter;
	GetDiskInfo(driveLetter,pCapacity,pFree,pUsed);
	
	
}



void DiskInfo::GetDiskInfo(CString driveLetter,ULARGE_INTEGER &Capacity,ULARGE_INTEGER &Free,ULARGE_INTEGER &Used)
{
	ULARGE_INTEGER AvailableToCaller;
	
	ZeroMemory(&AvailableToCaller, sizeof(ULARGE_INTEGER));
	
	InitMembers();

		GetDiskFreeSpaceExW(driveLetter, &AvailableToCaller,
									&Capacity, &Free);
		Used.QuadPart = Capacity.QuadPart - Free.QuadPart;

		
	
}
void DiskInfo::InitMembers(void)
{
	
	ZeroMemory(&pCapacity, sizeof(ULARGE_INTEGER));
	ZeroMemory(&pUsed, sizeof(ULARGE_INTEGER));
	ZeroMemory(&pFree, sizeof(ULARGE_INTEGER));

}