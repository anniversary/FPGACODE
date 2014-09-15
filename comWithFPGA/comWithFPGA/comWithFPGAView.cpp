
// comWithFPGAView.cpp : CcomWithFPGAView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "comWithFPGA.h"
#endif

#include "comWithFPGADoc.h"
#include "comWithFPGAView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcomWithFPGAView

IMPLEMENT_DYNCREATE(CcomWithFPGAView, CView)

BEGIN_MESSAGE_MAP(CcomWithFPGAView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CcomWithFPGAView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CcomWithFPGAView ����/����

CcomWithFPGAView::CcomWithFPGAView()
{
	// TODO: �ڴ˴���ӹ������

}

CcomWithFPGAView::~CcomWithFPGAView()
{
}

BOOL CcomWithFPGAView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CcomWithFPGAView ����

void CcomWithFPGAView::OnDraw(CDC* /*pDC*/)
{
	CcomWithFPGADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CcomWithFPGAView ��ӡ


void CcomWithFPGAView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CcomWithFPGAView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CcomWithFPGAView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CcomWithFPGAView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CcomWithFPGAView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CcomWithFPGAView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CcomWithFPGAView ���

#ifdef _DEBUG
void CcomWithFPGAView::AssertValid() const
{
	CView::AssertValid();
}

void CcomWithFPGAView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcomWithFPGADoc* CcomWithFPGAView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcomWithFPGADoc)));
	return (CcomWithFPGADoc*)m_pDocument;
}
#endif //_DEBUG


// CcomWithFPGAView ��Ϣ�������
