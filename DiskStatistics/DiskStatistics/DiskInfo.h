#pragma once

#include "PieView.h"
// DiskInfo

class DiskInfo : public CStatic
{
	DECLARE_DYNAMIC(DiskInfo)

public:
	DiskInfo();
	virtual ~DiskInfo();
	protected:
	CString pDiskLetter;
	
public:
	void InitMembers(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	void GetDiskInfo(CString driveLetter, ULARGE_INTEGER&  Capacity, ULARGE_INTEGER& Free, ULARGE_INTEGER& Usage);
	void DisplayDrive(CString driveLetter);
	ULARGE_INTEGER pCapacity;
	ULARGE_INTEGER pUsed;
	ULARGE_INTEGER pFree;
	CPieView pieView;
};


