#pragma once
#include "afxdialogex.h"
#include "CWMPPlayer4.h"


// Cvideo 对话框

class Cvideo : public CDialogEx
{
	DECLARE_DYNAMIC(Cvideo)

public:
	Cvideo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Cvideo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWMPPlayer4 m_mediaPlay;

	DECLARE_EVENTSINK_MAP()
	void Clickmediaplay(short nButton, short nShiftState, long fX, long fY);
};
