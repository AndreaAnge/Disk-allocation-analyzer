// FileView.cpp : implementation file
//

#include "stdafx.h"
#include "DiskStatistics.h"
#include "FileView.h"
#include "DiskStatisticsDoc.h"


// CFileView

IMPLEMENT_DYNCREATE(CFileView, CListView)

CFileView::CFileView()
: fileCount(0)
{

}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CFileView::OnLvnColumnclick)
END_MESSAGE_MAP()


// CFileView diagnostics

#ifdef _DEBUG
void CFileView::AssertValid() const
{
	CListView::AssertValid();
}

CDiskStatisticsDoc* CFileView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiskStatisticsDoc)));
	return (CDiskStatisticsDoc*)m_pDocument;
}
#ifndef _WIN32_WCE
void CFileView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileView message handlers


int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: Add your specialized creation code here
    GetDocument()->pFileView = this;

    return 0;
}


void CFileView::OnDraw(CDC* /*pDC*/)
{
	// TODO: Add your specialized code here and/or call the base class
	CDiskStatisticsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}


void CFileView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl &ctlFiles = this->GetListCtrl();
	
    ctlFiles.ModifyStyle(NULL, LVS_REPORT);
}


void CFileView::DisplaySelection(LPTSTR Path)
{
	CListCtrl &ctlFileView = this->GetListCtrl();

	Refresh();

	ctlFileView.DeleteAllItems();

	ctlFileView.InsertColumn(0, _T("File Name"), LVCFMT_LEFT, 200);
	ctlFileView.InsertColumn(1, _T("File Size"), LVCFMT_RIGHT, 90);
	ctlFileView.InsertColumn(2, _T("File Type"), LVCFMT_RIGHT, 80);


	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	int nItem;

	hFind = ::FindFirstFile(Path, &FindFileData);
	int n = 0;

	if (hFind != INVALID_HANDLE_VALUE){

		do {
			
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) 
					&& !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
				{
					if( FindFileData.cFileName != CString (".")
						&& FindFileData.cFileName != CString (".."))
					{
						
						++n;
						nItem = ctlFileView.InsertItem(n, _T("File"));  

						CString fName=FindFileData.cFileName;

						ctlFileView.SetItemText(nItem, 0, fName);

						/*CString fExt=GetExtension(fName);

						ctlFileView.SetItemText(nItem, 2, fExt);*/


					}
				}
			}

			else{
				++n;
				nItem = ctlFileView.InsertItem(n, _T("File"),4);

				CString name=FindFileData.cFileName;

				//AfxMessageBox(FindFileData.cFileName);

				ctlFileView.SetItemText(nItem, 0, name);

				ULARGE_INTEGER lFSize;

				lFSize.LowPart= FindFileData.nFileSizeLow;
				lFSize.HighPart=FindFileData.nFileSizeHigh;
				CString strFSize=_T("");


				//strFSize.Format(_T("%.2f"),lFSize.QuadPart/1024.0);

				if(lFSize.QuadPart>1024.0*1024.0*1024){
					strFSize.Format(_T("%1.2f GB"),lFSize.QuadPart/1024.0/1024.0/1024.0);
				}
				else if(lFSize.QuadPart>1024.0*1024.0){
					strFSize.Format(_T("%1.2f MB"),lFSize.QuadPart/1024.0/1024.0);
				}
				else if(lFSize.QuadPart>1024.0)
					strFSize.Format(_T("%1.2f KB"),lFSize.QuadPart/1024.0);
				else
					strFSize.Format(_T("%d bytes"),lFSize.QuadPart);

				ctlFileView.SetItemText(nItem, 1, strFSize); 

				CString ext=GetExtension(name);
				//sum['dir']['ext']=lfsize.quadpart
				ctlFileView.SetItemText(nItem, 2, ext);

			}

		}while((::WaitForSingleObject(m_hStopEvent, 0) !=
			WAIT_OBJECT_0) && (::FindNextFile(hFind, &FindFileData)));
		::FindClose(hFind);

		
	}
}

void CFileView::Refresh(void)
{
	CListCtrl &ctlLView = GetListCtrl();

	ctlLView.DeleteAllItems();
	while(ctlLView.DeleteColumn(0))
		;

	UpdateWindow();
}



CString CFileView::GetExtension(CString fileName)
{
	int i=fileName.ReverseFind(_T('.'));

	CString ext;

	if(i == -1) 
		ext=_T('.');
	else
		ext=fileName.Mid(i);

	return ext.MakeLower();
}

void CFileView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	
 //   GetListCtrl ().SortItems (CompareFunc, pNMLV->iSubItem);    //add compare
 //   *pResult = 0;

	
}
