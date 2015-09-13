// PieView.cpp : implementation file
//

#include "stdafx.h"
#include "DiskStatistics.h"
#include "DiskStatisticsDoc.h"
#include "PieView.h"
#include "math.h"
#include <Windows.h>
#include <GdiPlus.h>

#define DEFAULT_MARGIN		5
#define FREE_RGB			RGB(255, 20, 255)
#define USED_RGB			RGB(66, 66, 255)
#define WHITE_RGB			RGB(255, 255, 255)

// CPieView



IMPLEMENT_DYNCREATE(CPieView, CView)


CPieView::CPieView()
{
	InitMembers();
}

CPieView::~CPieView()
{
}

BEGIN_MESSAGE_MAP(CPieView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CPieView drawing

void CPieView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code here

	Draw2DPie(pDC);
}


// CPieView diagnostics

#ifdef _DEBUG
void CPieView::AssertValid() const
{
	CView::AssertValid();
}

CDiskStatisticsDoc* CPieView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiskStatisticsDoc)));
	return (CDiskStatisticsDoc*)m_pDocument;
}

#ifndef _WIN32_WCE
void CPieView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPieView message handlers


void CPieView::Draw2DPie(CDC* pDc)
{
	 
	/*if (pDiskLetter.IsEmpty())
		return;*/

	CRect rectArea;

	GetClientRect(&rectArea);
	rectArea.DeflateRect(2, 2);
	pDc->FillSolidRect(&rectArea, RGB(240,248,255));

	ULARGE_INTEGER Zero={0};

	//if (pCapacity.QuadPart==Zero.QuadPart)
	//	return;

	rectArea.top = rectArea.top+DEFAULT_MARGIN;
	rectArea.right = rectArea.right - DEFAULT_MARGIN;
	rectArea.left = rectArea.left + DEFAULT_MARGIN;
	rectArea.bottom = rectArea.bottom - DEFAULT_MARGIN;

	COLORREF clr[2];
	clr[0]=USED_RGB;
	clr[1]=FREE_RGB;

	CPen* pOldPen;
	CBrush* pOldBrush;

	////draw pie
	int CenterX, CenterY, radius, NewXLocation, NewYLocation, 
		lastXLocation, lastYLocation;
	CenterX=rectArea.left + rectArea.Width()/2;
	CenterY=rectArea.top + rectArea.Height()/2;

	radius=min(rectArea.Height(), rectArea.Width())/2;

	lastXLocation = CenterX + radius;
	lastYLocation = CenterY;

	CRect pieRect;
	pieRect.top = CenterY - radius;
	pieRect.left = CenterX - radius;
	pieRect.bottom = CenterY + radius;
	pieRect.right = CenterX + radius;

	////Slice[0] = Used
	////Slice[1] = Free
	double Slice[2]={0};
	Slice[0] = (double) (pUsed.QuadPart * 1.0 / pCapacity.QuadPart);
	Slice[1] = (double) (pFree.QuadPart * 1.0 / pCapacity.QuadPart);

	double degree=0, dataSum=0;

	for (int i=0; i<2; i++)
	{
		dataSum += Slice[i];
		degree = dataSum*2.0*3.141592;

		NewXLocation = CenterX + (int) ((double) radius*cos(degree));
		NewYLocation = CenterY - (int) ((double) radius*sin(degree));

		CPoint p1(lastXLocation, lastYLocation);
		CPoint p2(NewXLocation, NewYLocation);

		CBrush brush(clr[i]);
		CPen piePen(PS_SOLID, 1, clr[i]);
		pOldPen = pDc->SelectObject(&piePen);
		pOldBrush =pDc->SelectObject(&brush);

		pDc->Pie(pieRect, p1, p2);

		pDc->SelectObject(pOldBrush);
		pDc->SelectObject(pOldPen);

		lastXLocation = NewXLocation;
		lastYLocation = NewYLocation;
	}
}


void CPieView::InitMembers(void)
{
	
	ZeroMemory(&pCapacity, sizeof(ULARGE_INTEGER));
	ZeroMemory(&pUsed, sizeof(ULARGE_INTEGER));
	ZeroMemory(&pFree, sizeof(ULARGE_INTEGER));

}

// display drive information
void CPieView::DisplayDrive(CString driveLetter)
{
	
	pDiskLetter=driveLetter;
	GetDiskInfo(driveLetter,pCapacity,pFree,pUsed);
	//pDoc->pPieView->Invalidate();
	Invalidate();
	
}


void CPieView::GetDiskInfo(CString driveLetter,ULARGE_INTEGER &Capacity,ULARGE_INTEGER &Free,ULARGE_INTEGER &Used)
{
	ULARGE_INTEGER AvailableToCaller;
	
	ZeroMemory(&AvailableToCaller, sizeof(ULARGE_INTEGER));
	
	InitMembers();

		GetDiskFreeSpaceExW(driveLetter, &AvailableToCaller,
									&Capacity, &Free);
		Used.QuadPart = Capacity.QuadPart - Free.QuadPart;

		
	
}


int CPieView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	  
   GetDocument()->pPieView = this;
   
	return 0;
}

