#pragma once
class DiskSpaceInfo
{
public:
	ULARGE_INTEGER Used;
	ULARGE_INTEGER Capacity;
	ULARGE_INTEGER Free;

public:
	DiskSpaceInfo(void);
	~DiskSpaceInfo(void);
public:
	void InitializeMembers(void);
};

