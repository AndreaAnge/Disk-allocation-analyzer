// FolderSize.cpp : implementation file
//

#include "stdafx.h"
#include "DiskStatistics.h"
#include "FolderSize.h"


// FolderSize

IMPLEMENT_DYNAMIC(FolderSize, CStatic)

FolderSize::FolderSize()
{

}

FolderSize::~FolderSize()
{
}


BEGIN_MESSAGE_MAP(FolderSize, CStatic)
END_MESSAGE_MAP()



// FolderSize message handlers




BOOL FolderSize::GetFolderSize(LPCTSTR Path, int* folderCount, int* fileCount, ULARGE_INTEGER& folderSize)
{
	
	if(_tcslen(Path) < 4)
		return FALSE;


		//if ((_tcslen(Path) < 4))
		//{
		//	//za cijeli disk

		//	ULARGE_INTEGER ulFreeCaller;
		//	ulFreeCaller.QuadPart = 0;
		//	ULARGE_INTEGER ulTotal;
		//	ulTotal.QuadPart = 0;
		//	ULARGE_INTEGER ulFree;
		//	ulFree.QuadPart = 0;

		//	rc = GetDiskFreeSpaceEx(lpszStartFolder, &ulFreeCaller, &ulTotal, &ulFree);
		//	lpFolderSize->QuadPart = ulTotal.QuadPart - ulFree.QuadPart;
		//	return rc;
		//}

		HANDLE hFF = 0;
		WIN32_FIND_DATA wfd = { 0 };
		TCHAR szPath[2000];
		ULARGE_INTEGER li_total;
		li_total.QuadPart = 0;
		int dwTotalFolderCount = 0;
		int dwTotalFileCount = 0;

		_tcsncpy(szPath, Path, sizeof(szPath)/sizeof(TCHAR)-1);
		_tcscat(szPath, _T("\\*"));
		
		hFF = FindFirstFile(szPath, &wfd);
		
		if (hFF != INVALID_HANDLE_VALUE)
		{
			

			do
			{
				_tcsncpy(szPath, Path, sizeof(szPath)/sizeof(TCHAR)-1);
				_tcscat(szPath, _T("\\"));
				_tcscat(szPath, wfd.cFileName);

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (_tcscmp(wfd.cFileName, _T(".")) &&
						_tcscmp(wfd.cFileName, _T("..")) 
						)
					{
						// rekurzija ako je direktorij
						
						ULARGE_INTEGER li;
						li.QuadPart = 0;
						int dwFolderCount = 0;
						int dwFileCount = 0;
						GetFolderSize(szPath,&dwFolderCount, &dwFileCount, li);
								
						li_total.QuadPart += li.QuadPart;
						dwTotalFolderCount += dwFolderCount + 1;
						dwTotalFileCount += dwFileCount;
					}
				}
				else
				{
					LARGE_INTEGER li;
					li.HighPart=wfd.nFileSizeHigh;
					li.LowPart=wfd.nFileSizeLow;
					//rc = GetFileSize(szPath, &li);
					
					li_total.QuadPart += li.QuadPart;
					dwTotalFileCount += 1;
				}

			} while (FindNextFile(hFF, &wfd));
		}

		folderSize = li_total;
		if (folderCount)
			*folderCount = dwTotalFolderCount;
		if (fileCount)
			*fileCount = dwTotalFileCount;

	return TRUE;

}
