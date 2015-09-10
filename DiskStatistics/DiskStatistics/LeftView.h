#pragma once

class CDiskStatisticsDoc;
// CLeftView view

class CLeftView : public CTreeView
{
	DECLARE_DYNCREATE(CLeftView)

protected:
	CLeftView();           // protected constructor used by dynamic creation
	virtual ~CLeftView();
public:
	CDiskStatisticsDoc* GetDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void CreateRoots(void);
	CString GetPathFromItem(HTREEITEM hItem);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
private:
	HANDLE m_hStopEvent;
public:
	void GetPath(CString CStrPath);
};


#ifndef _DEBUG  
inline CFTreeBrowserDoc* CLeftView::GetDocument()
{ return (CFTreeBrowserDoc*)m_pDocument; }
#endif