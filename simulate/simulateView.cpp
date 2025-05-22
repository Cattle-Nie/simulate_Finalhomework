
// simulateView.cpp: CsimulateView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "simulate.h"
#endif

#include "simulateDoc.h"
#include "simulateView.h"
#include "simulateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsimulateView

IMPLEMENT_DYNCREATE(CsimulateView, CView)

BEGIN_MESSAGE_MAP(CsimulateView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EXP, &CsimulateView::OnExp)
END_MESSAGE_MAP()

// CsimulateView 构造/析构

CsimulateView::CsimulateView() noexcept
{
	// TODO: 在此处添加构造代码

}

CsimulateView::~CsimulateView()
{
}

BOOL CsimulateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CsimulateView 绘图

void CsimulateView::OnDraw(CDC* /*pDC*/)
{
	CsimulateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CsimulateView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CsimulateView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CsimulateView 诊断

#ifdef _DEBUG
void CsimulateView::AssertValid() const
{
	CView::AssertValid();
}

void CsimulateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CsimulateDoc* CsimulateView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CsimulateDoc)));
	return (CsimulateDoc*)m_pDocument;
}
#endif //_DEBUG


// CsimulateView 消息处理程序

void CsimulateView::OnExp()
{
	// TODO: 在此添加命令处理程序代码
	CsimulateDlg dlg;
	dlg.DoModal();
}
