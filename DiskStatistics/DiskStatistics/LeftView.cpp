// LeftView.cpp : implementation file
//

#include "stdafx.h"
#include "DiskStatistics.h"
#include "DiskStatisticsDoc.h"
#include "LeftView.h"
#include "FileView.h"
#include "PieView.h"


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

CLeftView::CLeftView()
{

}

CLeftView::~CLeftView()
{
}

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_WM_CREATE()
	
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

CDiskStatisticsDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiskStatisticsDoc)));
	return (CDiskStatisticsDoc*)m_pDocument;
}

#ifndef _WIN32_WCE
void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftView message handlers


void CLeftView::OnDraw(CDC* /*pDC*/)
{
	// TODO: Add your specialized code here and/or call the base class
	CDiskStatisticsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}


BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	    cs.style |= TVS_HASLINES  | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

		return  CTreeView::PreCreateWindow(cs);
}


void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: Add your specialized creation code here
    GetDocument()->pLeftView = this;
    return 0;
}


void CLeftView::CreateRoots()
{
	CTreeCtrl &ctlTree=GetTreeCtrl();
	ctlTree.DeleteAllItems();

	CTreeCtrl &ctlDrives = this->GetTreeCtrl();

	HTREEITEM hRoot;

	char * strBuffer= NULL;
	CString strMessage;

	int nPos = 0;
    UINT nCount = 0;
    CString strDrive = _T("?:\\");

    DWORD dwDriveList = ::GetLogicalDrives ();

    CString cTmp;

    while (dwDriveList) {
        if (dwDriveList & 1) {
            cTmp = strDrive;
            strDrive.SetAt (0, 0x41 + nPos);

            strDrive = strDrive.Left(2);
            hRoot = ctlDrives.InsertItem(strDrive,0, 1);
        }
        dwDriveList >>= 1;
        nPos++;
    }

	 TCHAR szPath[MAX_PATH];
    ::GetCurrentDirectory (sizeof (szPath) / sizeof (TCHAR), szPath);
    CString strPath = szPath;
    strPath = strPath.Left (3);

    HTREEITEM hItem = GetTreeCtrl ().GetNextItem (NULL, TVGN_ROOT);
    while (hItem != NULL) {
        if (GetTreeCtrl ().GetItemText (hItem) == strPath)
            break;
        hItem = GetTreeCtrl ().GetNextSiblingItem (hItem);
    }

    if (hItem != NULL) {
        GetTreeCtrl ().Expand (hItem, TVE_EXPAND);
        GetTreeCtrl ().Select (hItem, TVGN_CARET);
    }

    strPath = GetPathFromItem (GetTreeCtrl().GetSelectedItem());
    GetDocument()->UpdateAllViews (this, 0x5A, 
        (CObject*) (LPCTSTR) strPath);

}



CString CLeftView::GetPathFromItem(HTREEITEM hItem)
{
	CString strResult = GetTreeCtrl().GetItemText (hItem);

    HTREEITEM hParent;
    while ((hParent = GetTreeCtrl().GetParentItem (hItem)) != NULL) {
        CString string = GetTreeCtrl().GetItemText (hParent);

   if (string.Right (1) != _T ("\\"))
            string += _T ("\\");
        strResult = string + strResult;
        hItem = hParent;
    }
    return strResult;
}



void CLeftView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CDiskStatisticsDoc *pDoc = GetDocument();

    // reference to the tree control
    CTreeCtrl &ctlFiles = this->GetTreeCtrl();

    
    HTREEITEM nodSelected = ctlFiles.GetSelectedItem(); //c: 
    
    CString strSelected = ctlFiles.GetItemText(nodSelected);

    HTREEITEM nodParent = nodSelected;

    
    do {
            nodParent = ctlFiles.GetParentItem(nodParent);
            if (nodParent!=NULL)
                strSelected = ctlFiles.GetItemText(nodParent) + "\\" + strSelected;
    } while (nodParent != NULL);

    CString strSearchPath = strSelected + ("\\*.*"); //c:\*.*
	//CString strDrive = strSelected + ("\\");

    pDoc->pFileView->DisplaySelection(strSearchPath.GetBuffer(1));

    GetPath(strSelected); // expands the selected drive

	pDoc->pPieView->DisplayDrive(strSelected);
	
}


void CLeftView::GetPath(CString CStrPath)
{
	CTreeCtrl &ctlFolders = this->GetTreeCtrl();
	HTREEITEM hRoot;
	HTREEITEM hFolder;

	hRoot = ctlFolders.GetSelectedItem();


	HTREEITEM hChild = ctlFolders.GetChildItem(hRoot);

	while(hChild){
		ctlFolders.DeleteItem(hChild);
		hChild = ctlFolders.GetChildItem(hRoot);
		}

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	
	CStrPath += "\\*.*";

	hFind = FindFirstFile(CStrPath, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE){
		do {

			long lFSize = FindFileData.nFileSizeLow;
			CString strFSize=_T("");

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
				&& FindFileData.cFileName != CString (".")
				&& FindFileData.cFileName != CString (".."))
				hFolder = ctlFolders.InsertItem
					(FindFileData.cFileName,2,3,hRoot);

		}while((::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0) 
		&& (::FindNextFile(hFind, &FindFileData)));
				
		::FindClose(hFind);;
	}
}
