
// simulateDlg.h: 头文件
//

#pragma once


// CsimulateDlg 对话框
class CsimulateDlg : public CDialogEx
{
	// 构造
public:
	CsimulateDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMULATE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedStart();
	CBitmap Background;     // 缓存的背景
	//CBitmap Draw;
	CDC MemDC;                 // 内存DC
	//CDC DrawDC;
	CRect rc;					 //模拟绘图区域
	int n_angle;
	int n_D1;
	int n_D2;
	int n_v;
	double n_miu;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	CEdit edit1;
	CEdit edit2;
	CEdit edit3;

	CScrollBar bar_d1;
	CSliderCtrl sld_d2;

	void Changeedit1();
	void Changeedit2();
	void Changeedit3();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CComboBox m_cb;//组框
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	HBRUSH hBrush;
	CButton m_chk1;
	CButton m_chk_key;
	CButton m_chk_nog;
	afx_msg void OnBnClickedCheckNoelost();
	afx_msg void OnBnClickedCheckKeyctrl();
	afx_msg void OnBnClickedCheckNog();
	CStatic t_start;
	CStatic t_v;
	CStatic t_vx;
	CStatic t_vy;
	CStatic t_a;
	CStatic t_ax;
	CStatic t_ay;
	CStatic t_position;
};
