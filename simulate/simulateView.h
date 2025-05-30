
// simulateView.h: CsimulateView 类的接口
//

#pragma once


class CsimulateView : public CView
{
protected: // 仅从序列化创建
	CsimulateView() noexcept;
	DECLARE_DYNCREATE(CsimulateView)

// 特性
public:
	CsimulateDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CsimulateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	CMenu m_PopMenu;
	CMenu* m_pPOP;
protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnExp();

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExp1();
	afx_msg void OnExp2();
	afx_msg void OnExp3();
};

#ifndef _DEBUG  // simulateView.cpp 中的调试版本
inline CsimulateDoc* CsimulateView::GetDocument() const
   { return reinterpret_cast<CsimulateDoc*>(m_pDocument); }
#endif

