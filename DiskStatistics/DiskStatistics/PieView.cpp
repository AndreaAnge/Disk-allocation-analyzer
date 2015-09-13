// PieView.cpp : implementation file
//

#include "stdafx.h"
#include "DiskStatistics.h"
#include "PieView.h"
#include "math.h"
#include <Windows.h>
#include <GdiPlus.h>
#include "FolderSize.h"
#include "DiskStatisticsDoc.h"

#define DEFAULT_MARGIN		5
#define FREE_RGB			RGB(219, 145, 121)
#define USED_RGB			RGB(121, 195, 219)
#define WHITE_RGB			RGB(255, 255, 255)
#define THIS_RGB			RGB(60,60,86)

// CPieView



IMPLEMENT_DYNCREATE(CPieView, CView)


CPieView::CPieView()
: folderCount(0)
, fileCount(0)
{
	InitMembers();
}

CPieView::~CPieView()
{
}

BEGIN_MESSAGE_MAP(CPieView, CView)
	ON_WM_CREATE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


// CPieView drawing

void CPieView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code here

	Draw2DPie(pDC);
	DrawText(pDC);
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
	pDc->FillSolidRect(&rectArea, WHITE_RGB);

	ULARGE_INTEGER Zero={0};

	//if (pCapacity.QuadPart==Zero.QuadPart)
	//	return;

	rectArea.top = rectArea.top+DEFAULT_MARGIN;
	rectArea.right = rectArea.right - DEFAULT_MARGIN;
	rectArea.left = rectArea.left + DEFAULT_MARGIN;
	rectArea.bottom = rectArea.bottom - DEFAULT_MARGIN;

	COLORREF clr[2];
	clr[0]=FREE_RGB;
	clr[1]=USED_RGB;
	
	

	CPen* pOldPen;
	CBrush* pOldBrush;


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

	

	
	double Slice[2]={0};
	Slice[0] = (double) (pFree.QuadPart * 1.0 / pCapacity.QuadPart);
	Slice[1] = (double) (pUsed.QuadPart * 1.0 / pCapacity.QuadPart);
	

	
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
	ZeroMemory(&pOther, sizeof(ULARGE_INTEGER));

}

// display drive information
void CPieView::DisplayDrive(CString driveLetter)
{
	
	pDiskLetter=driveLetter;
	GetDiskInfo(driveLetter,pCapacity,pFree,pUsed);

		
	folderSize.GetFolderSize(driveLetter,&folderCount,&fileCount,pOther);

	Invalidate();
	
}


void CPieView::GetDiskInfo(CString driveLetter,ULARGE_INTEGER &Capacity,ULARGE_INTEGER &Free,ULARGE_INTEGER &Used)
{
	ULARGE_INTEGER AvailableToCaller;
	
	ZeroMemory(&AvailableToCaller, sizeof(ULARGE_INTEGER));
	
	InitMembers();

		GetDiskFreeSpaceExW((driveLetter).Left(3), &AvailableToCaller,
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



void CPieView::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnMouseHover(nFlags, point);
}


void CPieView::DrawText(CDC* pDc)
{
	
	CRect rectArea;

	GetClientRect(&rectArea);

	rectArea.top = rectArea.top + DEFAULT_MARGIN;
	rectArea.right = rectArea.right - DEFAULT_MARGIN;
	rectArea.left = rectArea.left + DEFAULT_MARGIN;
	rectArea.bottom = rectArea.bottom - rectArea.Height()*3/5 - DEFAULT_MARGIN;

	CRect Line1, Line2, Line3, Line4, Line5, Line6;
	Line1 = Line2 = Line3 = Line4 =Line5=Line6= rectArea;
	Line1.bottom = rectArea.top + rectArea.Height()/3;
	Line2.top = Line1.bottom;
	Line2.bottom = rectArea.bottom - rectArea.Height()/3;
	Line3.top = Line2.bottom;
	Line4.top=Line3.bottom;
	Line5.top=rectArea.bottom + rectArea.Height()/5;
	Line6.top=Line5.bottom + rectArea.Height()-85;


	CFont FontTitle;
	FontTitle.CreateFont(15, 0, 0, 0, 0, FALSE,FALSE, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
	
	CFont* pOldFont=(CFont*) pDc->SelectObject(&FontTitle);

	CString Title;
	ULARGE_INTEGER temp={0};

	if (pCapacity.QuadPart > 1024.0*1024.0*1024)
		Title.Format(_T("Capacity: %2.2f GB"), pCapacity.QuadPart/1024.0/1024.0/1024.0);
	else if (pCapacity.QuadPart > 1024.0*1024.0)
		Title.Format(_T("Capacity: %2.2f MB"), pCapacity.QuadPart/1024.0/1024.0);
	else
		Title.Format(_T("Capacity: %2.2f KB"), pCapacity.QuadPart/1024.0);
	
	pDc->TextOut(Line1.left + DEFAULT_MARGIN + 4.5,
				Line1.top + DEFAULT_MARGIN,
				Title);

	if (pUsed.QuadPart > 1024.0*1024.0*1024)
		Title.Format(_T("Used: %2.2f GB"), pUsed.QuadPart/1024.0/1024.0/1024.0);
	else if (pUsed.QuadPart > 1024.0*1024.0)
		Title.Format(_T("Used: %2.2f MB"), pUsed.QuadPart/1024.0/1024.0);
	else
		Title.Format(_T("Used: %2.2f KB"), pUsed.QuadPart/1024.0);
	
	pDc->TextOut(Line2.left + Line2.Width()/20,
				Line2.top,
				Title);

	if (pFree.QuadPart > 1024.0*1024.0*1024)
		Title.Format(_T("Free: %2.2f GB"), pFree.QuadPart/1024.0/1024.0/1024.0);
	else if (pFree.QuadPart > 1024.0*1024.0)
		Title.Format(_T("Free: %2.2f MB"), pFree.QuadPart/1024.0/1024.0);
	else
		Title.Format(_T("Free: %2.2f KB"), pFree.QuadPart/1024.0);
	
	pDc->TextOut(Line3.left + Line3.Width()/20,
				Line3.top - DEFAULT_MARGIN,
				Title);

	if (pOther.QuadPart > 1024.0*1024.0*1024)
		Title.Format(_T("Folder Size: %2.2f GB"), pOther.QuadPart/1024.0/1024.0/1024.0);
	else if (pOther.QuadPart > 1024.0*1024.0)
		Title.Format(_T("Folder Size: %2.2f MB"), pOther.QuadPart/1024.0/1024.0);
	else
		Title.Format(_T("Folder Size: %2.2f KB"), pOther.QuadPart/1024.0);
	
	pDc->TextOut(Line4.left + Line4.Width()/20,
				Line4.top - DEFAULT_MARGIN,
				Title);
	
	Title.Format(_T("# of folders: %d"), folderCount);
	pDc->TextOut(Line5.left + Line5.Width()/20,
				Line5.top,
				Title);

	Title.Format(_T("# of files: %d"), fileCount);
	pDc->TextOut(Line6.left + Line6.Width()/20,
				Line6.top,
				Title);

	pDc->SelectObject(pOldFont);

	CBrush UsedBrush, FreeBrush, FolderBrush;
	CRect FreeRect, UsedRect, FolderRect;

	//kockice 
	UsedRect.top=Line2.top;
	UsedRect.left=Line2.left + DEFAULT_MARGIN * 2;
	UsedRect.right=UsedRect.left + DEFAULT_MARGIN * 3 - 2;
	UsedRect.bottom=UsedRect.top + DEFAULT_MARGIN * 3 - 2;
	UsedBrush.CreateSolidBrush(USED_RGB);
	pDc->FillRect(&UsedRect,&UsedBrush);

	FreeRect.top=Line3.top - DEFAULT_MARGIN;
	FreeRect.left=Line3.left + DEFAULT_MARGIN * 2;
	FreeRect.right=FreeRect.left + DEFAULT_MARGIN * 3 - 2;
	FreeRect.bottom=FreeRect.top + DEFAULT_MARGIN * 3 - 2;
	FreeBrush.CreateSolidBrush(FREE_RGB);
	pDc->FillRect(&FreeRect,&FreeBrush);

	FolderRect.top=Line4.top - DEFAULT_MARGIN;
	FolderRect.left=Line4.left + DEFAULT_MARGIN * 2;
	FolderRect.right=FolderRect.left + DEFAULT_MARGIN * 3 - 2;
	FolderRect.bottom=FolderRect.top + DEFAULT_MARGIN * 3 - 2;
	FolderBrush.CreateSolidBrush(THIS_RGB);
	pDc->FillRect(&FolderRect,&FolderBrush);
}
