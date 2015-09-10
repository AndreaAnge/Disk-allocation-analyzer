#pragma once


// CFileView view
class CDiskStatisticsDoc;

class CFileView : public CListView
{
	DECLARE_DYNCREATE(CFileView)

protected:
	CFileView();           // protected constructor used by dynamic creation
	virtual ~CFileView();

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
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* /*pDC*/);
	virtual void OnInitialUpdate();
	//virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	void DisplaySelection(LPTSTR Path);

private:
	HANDLE m_hStopEvent;


};


#ifndef _DEBUG  
inline CDiskStatisticsDoc* CFileView::GetDocument()
{ return (CDiskStatisticsDoc*)m_pDocument; }
#endif