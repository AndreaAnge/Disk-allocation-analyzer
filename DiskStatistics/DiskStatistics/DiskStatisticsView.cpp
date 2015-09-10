
// DiskStatisticsView.cpp : implementation of the CDiskStatisticsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DiskStatistics.h"
#endif

#include "DiskStatisticsDoc.h"
#include "DiskStatisticsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDiskStatisticsView

IMPLEMENT_DYNCREATE(CDiskStatisticsView, CView)

BEGIN_MESSAGE_MAP(CDiskStatisticsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDiskStatisticsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDiskStatisticsView construction/destruction

CDiskStatisticsView::CDiskStatisticsView()
{
	// TODO: add construction code here

}

CDiskStatisticsView::~CDiskStatisticsView()
{
}

BOOL CDiskStatisticsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDiskStatisticsView drawing

void CDiskStatisticsView::OnDraw(CDC* /*pDC*/)
{
	CDiskStatisticsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDiskStatisticsView printing


void CDiskStatisticsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDiskStatisticsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDiskStatisticsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDiskStatisticsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDiskStatisticsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDiskStatisticsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDiskStatisticsView diagnostics

#ifdef _DEBUG
void CDiskStatisticsView::AssertValid() const
{
	CView::AssertValid();
}

void CDiskStatisticsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDiskStatisticsDoc* CDiskStatisticsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiskStatisticsDoc)));
	return (CDiskStatisticsDoc*)m_pDocument;
}
#endif //_DEBUG


// CDiskStatisticsView message handlers
