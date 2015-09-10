// FileView.cpp : implementation file
//

#include "stdafx.h"
#include "DiskStatistics.h"
#include "FileView.h"
#include "DiskStatisticsDoc.h"


// CFileView

IMPLEMENT_DYNCREATE(CFileView, CListView)

CFileView::CFileView()
{

}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CListView)
	ON_WM_CREATE()
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

    //ResetFiles();

	ctlFileView.DeleteAllItems ();

    ctlFileView.InsertColumn(0, _T("File Name"), LVCFMT_LEFT, 190);
    ctlFileView.InsertColumn(1, _T("File Size"), LVCFMT_RIGHT, 80);
	ctlFileView.InsertColumn(1, _T("File Type"), LVCFMT_RIGHT, 80);


    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    int nItem;

	hFind = FindFirstFile(Path, &FindFileData);
    int n = 0;

    if (hFind != INVALID_HANDLE_VALUE){

        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
                && FindFileData.cFileName != CString (".")
                && FindFileData.cFileName != CString (".."))
                {
                ++n;
                nItem = ctlFileView.InsertItem(n, _T("File"));  //prvi parametar je indeks


                ctlFileView.SetItemText(nItem, 0, FindFileData.cFileName);
				

                long lFSize = FindFileData.nFileSizeLow;
                CString strFSize=_T("");
                strFSize.Format(_T("%u"),lFSize);

                ctlFileView.SetItemText(nItem, 2, strFSize);  //strFSize.GetBuffer(1)
            }
        }while((::WaitForSingleObject(m_hStopEvent, 0) !=
                WAIT_OBJECT_0) && (::FindNextFile(hFind, &FindFileData)));
                ::FindClose(hFind);;

}
}