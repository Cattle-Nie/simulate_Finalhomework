
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
	//函数原型，由系统生成，这里不注释，功能在cpp文件写了
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckNoelost();
	afx_msg void OnBnClickedCheckKeyctrl();
	afx_msg void OnBnClickedCheckNog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Changeedit1();
	void Changeedit2();
	void Changeedit3();

	//变量
	CBitmap Background;     // 缓存的背景
	CDC MemDC;              // 缓存背景的内存DC
	CRect rc;				//模拟绘图区域
	HBRUSH hBrush;			// 画刷

	int n_angle; //发射角度
	int n_D1; //小球直径
	int n_D2; //障碍直径
	int n_v; //初速度
	double n_miu; //摩擦系数

	CEdit edit1; //发射角度编辑框
	CEdit edit2; //小球直径编辑框
	CEdit edit3; //障碍直径编辑框
	CScrollBar bar_d1; //小球直径滚动条
	CSliderCtrl sld_d2; //障碍直径滑竿
	CComboBox m_cb; //空气阻力下拉框
	CButton m_chk1; //弹性碰撞复选框
	CButton m_chk_key; //方向键控制复选框
	CButton m_chk_nog; //无重力复选框

	//用于监视的静态控件
	CStatic t_start;
	CStatic t_v;
	CStatic t_vx;
	CStatic t_vy;
	CStatic t_a;
	CStatic t_ax;
	CStatic t_ay;
	CStatic t_position;
};
