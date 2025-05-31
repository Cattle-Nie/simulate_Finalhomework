
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
#include "Cvideo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsimulateView

IMPLEMENT_DYNCREATE(CsimulateView, CView)

BEGIN_MESSAGE_MAP(CsimulateView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EXP, &CsimulateView::OnExp)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EXP1, &CsimulateView::OnExp1)
	ON_COMMAND(ID_EXP2, &CsimulateView::OnExp2)
	ON_COMMAND(ID_EXP3, &CsimulateView::OnExp3)
	ON_COMMAND(ID_MEDIAD, &CsimulateView::OnMediad)
END_MESSAGE_MAP()

// CsimulateView 构造/析构

CsimulateView::CsimulateView() noexcept
{
	// TODO: 在此处添加构造代码
	m_PopMenu.LoadMenu(IDR_POP_EXP);
}

CsimulateView::~CsimulateView()
{
	m_PopMenu.DestroyMenu();
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
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
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
	dlg.n_angle = 15;
	dlg.n_D1 = 50;
	dlg.n_D2 = 100;
	dlg.n_v = 20;
	dlg.n_miu = 0.1;
	dlg.DoModal();
}
//右键单击函数，用于唤起快捷菜单
void CsimulateView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pPOP = m_PopMenu.GetSubMenu(0);
	ClientToScreen(&point);
	m_pPOP->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	CView::OnRButtonDown(nFlags, point);
}


void CsimulateView::OnExp1()
{
	// TODO: 在此添加命令处理程序代码
	CsimulateDlg dlg1;
	dlg1.n_angle = 45;
	dlg1.n_D1 = 100;
	dlg1.n_D2 = 100;
	dlg1.n_miu = 0.3;
	dlg1.n_v = 30;
	dlg1.DoModal();
}

void CsimulateView::OnExp2()
{
	// TODO: 在此添加命令处理程序代码
	CsimulateDlg dlg2;
	dlg2.n_angle = 75;
	dlg2.n_D1 = 100;
	dlg2.n_D2 = 200;
	dlg2.n_miu = 0.2;
	dlg2.n_v = 25;
	dlg2.DoModal();
}

void CsimulateView::OnExp3()
{
	// TODO: 在此添加命令处理程序代码
	CsimulateDlg dlg3;
	dlg3.n_angle = 60;
	dlg3.n_D1 = 80;
	dlg3.n_D2 = 150;
	dlg3.n_miu = 0.1;
	dlg3.n_v = 15;
	dlg3.DoModal();
}

void CsimulateView::OnMediad()
{
	// TODO: 在此添加命令处理程序代码
	Cvideo dlgv;
	dlgv.DoModal();
}
