
// comWithFPGAView.cpp : CcomWithFPGAView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CcomWithFPGAView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CcomWithFPGAView 构造/析构

CcomWithFPGAView::CcomWithFPGAView()
{
	// TODO: 在此处添加构造代码

}

CcomWithFPGAView::~CcomWithFPGAView()
{
}

BOOL CcomWithFPGAView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CcomWithFPGAView 绘制

void CcomWithFPGAView::OnDraw(CDC* /*pDC*/)
{
	CcomWithFPGADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CcomWithFPGAView 打印


void CcomWithFPGAView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CcomWithFPGAView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CcomWithFPGAView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CcomWithFPGAView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CcomWithFPGAView 诊断

#ifdef _DEBUG
void CcomWithFPGAView::AssertValid() const
{
	CView::AssertValid();
}

void CcomWithFPGAView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcomWithFPGADoc* CcomWithFPGAView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcomWithFPGADoc)));
	return (CcomWithFPGADoc*)m_pDocument;
}
#endif //_DEBUG


// CcomWithFPGAView 消息处理程序
