#pragma once




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
	void GetDiskInfo(CString driveLetter,ULARGE_INTEGER &Capacity, ULARGE_INTEGER &Used, ULARGE_INTEGER &Free);

private:
	ULARGE_INTEGER pUsed;
	ULARGE_INTEGER pCapacity;
	ULARGE_INTEGER pFree;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	void DrawText(CDC* pDc);
};

#ifndef _DEBUG  
inline CDiscStatisticsDoc* GetDocument()
{ return reinterpret_cast<CDiskStatisticsDoc*>(m_pDocument); }
#endif