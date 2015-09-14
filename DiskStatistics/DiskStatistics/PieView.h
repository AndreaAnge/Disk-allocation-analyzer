#pragma once

#include "DiskStatisticsDoc.h"
#include "foldersize.h"



// CPieView view
class CDiskStatisticsDoc;

class CPieView : public CView
{
	DECLARE_DYNCREATE(CPieView)

public:
	CPieView();           // protected constructor used by dynamic creation
	virtual ~CPieView();

	public:
	CDiskStatisticsDoc* GetDocument();
public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Draw2DPie(CDC* );

public:
	void InitMembers(void);

	// display drive information
	void DisplayDrive(CString driveLetter);
protected:
	CString pDiskLetter;
public:
	void GetDiskInfo(CString driveLetter);

private:
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	void DrawText(CDC* pDc);
private:
	ULARGE_INTEGER pOther;
	ULARGE_INTEGER pUsed;
	ULARGE_INTEGER pCapacity;
	ULARGE_INTEGER pFree;
	FolderSize folderSize;
public:
	int folderCount;
	int fileCount;
};

#ifndef _DEBUG  
inline CDiscStatisticsDoc* CPieView::GetDocument()
{ return (CDiskStatisticsDoc*)m_pDocument; }
#endif