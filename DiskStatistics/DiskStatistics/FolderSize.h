#pragma once


// FolderSize

class FolderSize : public CStatic
{
	DECLARE_DYNAMIC(FolderSize)

public:
	FolderSize();
	virtual ~FolderSize();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL GetFolderSize(LPCTSTR Path, int* folderCount, int* fileCount, ULARGE_INTEGER& folderSize);
};


