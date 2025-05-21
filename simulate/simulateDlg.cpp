
// simulateDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "simulate.h"
#include "simulateDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897

// CsimulateDlg 对话框
//全局变量
BOOL start = 0;//模拟开始开关
CPoint ball1,ball2;//小球的左上点和右下点
CPoint Ball1,Ball2;//障碍左上点和右下点
double w, h, x, y, X0, Y0, r, R, vx, vy, ax, ay;
//画布长，高，小球障碍中心位置xy，小球半径r，障碍半径R，x和y速度v，加速度a；
double v; //这是绝对速度
int t;//时间变量
int count=0;
double g = 1.0, k = 1; //重力加速度，弹性系数
BOOL key;//自选参数：方向键开关.
COLORREF Colors[7] = {
	RGB(255, 0, 0),      // 红
	RGB(255, 128, 0),    // 橙
	RGB(255, 255, 0),    // 黄
	RGB(0, 255, 0),      // 绿
	RGB(0, 0, 255),      // 蓝
	RGB(75, 0, 130),     // 靛
	RGB(139, 0, 255)     // 紫
};//障碍颜色

CsimulateDlg::CsimulateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIMULATE_DIALOG, pParent)
	, n_angle(0), 
	n_D1(0), 
	n_D2(0), 
	n_miu(0), 
	n_v(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsimulateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, n_angle);
	DDX_Control(pDX, IDC_EDIT1, edit1);
	DDX_Control(pDX, IDC_BAR_D1, bar_d1);
	DDX_Control(pDX, IDC_SLD_D2, sld_d2);
	DDX_Control(pDX, IDC_EDIT2, edit2);
	DDX_Control(pDX, IDC_EDIT3, edit3);
	DDX_Control(pDX, IDC_COMBO1, m_cb);
	DDX_Control(pDX, IDC_CHECK_NOELOST, m_chk1);
	DDX_Control(pDX, IDC_CHECK_KEYCTRL, m_chk_key);
	DDX_Control(pDX, IDC_CHECK_NOG, m_chk_nog);
	DDX_Control(pDX, IDT_START, t_start);
	DDX_Control(pDX, IDT_V, t_v);
	DDX_Control(pDX, IDT_VX, t_vx);
	DDX_Control(pDX, IDT_VY, t_vy);
	DDX_Control(pDX, IDT_A, t_a);
	DDX_Control(pDX, IDT_AX, t_ax);
	DDX_Control(pDX, IDT_AY, t_ay);
	DDX_Control(pDX, IDT_POSITION, t_position);
}

BEGIN_MESSAGE_MAP(CsimulateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_EXIT, &CsimulateDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_START, &CsimulateDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_RADIO1, &CsimulateDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CsimulateDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CsimulateDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CsimulateDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CsimulateDlg::OnBnClickedRadio5)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CsimulateDlg::OnCbnSelchangeCombo1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_NOELOST, &CsimulateDlg::OnBnClickedCheckNoelost)
	ON_BN_CLICKED(IDC_CHECK_KEYCTRL, &CsimulateDlg::OnBnClickedCheckKeyctrl)
	ON_BN_CLICKED(IDC_CHECK_NOG, &CsimulateDlg::OnBnClickedCheckNog)
END_MESSAGE_MAP()


// CsimulateDlg 消息处理程序

BOOL CsimulateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//控件操作
	n_angle = 15;
	n_D1 = 50;
	n_D2 = 100;
	n_v = 20;
	n_miu = 0.1;

	bar_d1.SetScrollRange(40,100);
	bar_d1.SetScrollPos(n_D1);
	sld_d2.SetRange(50, 200);
	sld_d2.SetPos(n_D2);

	Changeedit1();
	Changeedit2();
	Changeedit3();

	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	pSpin->SetRange(10, 30);
	pSpin->SetPos(20);
	pSpin->GetBuddy()->SetWindowText(L"20");

	m_cb.AddString(L"0.1"); 
	m_cb.AddString(L"0.2");
	m_cb.AddString(L"0.3");
	m_cb.AddString(L"无空气阻力");
	m_cb.SetCurSel(0); // 默认选中第一项

	m_chk1.SetCheck(1); // 默认选中

	//绘图操作
	CClientDC dc(this);
	CRect tmprc;
	GetDlgItem(IDC_RECT)->GetWindowRect(&tmprc);
	ScreenToClient(&tmprc); // 转换为对话框内部坐标
	rc = tmprc; // 保存矩形区域
	// 创建兼容DC
	MemDC.CreateCompatibleDC(&dc);
	// 创建背景Bitmap
	Background.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	// 将Bitmap选入DC
	CBitmap* pOldBmp = MemDC.SelectObject(&Background);
	// 填充白色背景
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 255, 255)); 
	MemDC.SelectObject(pOldBmp); // 恢复原来的Bitmap
	Invalidate(FALSE); // 触发 OnPaint，不清除背景
	UpdateWindow();    // 立即执行绘图（不等待消息循环）

	SetTimer(1, 30, NULL);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE


}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsimulateDlg::OnPaint()
{
	ball1.x = x - r;
	ball1.y = h - (y + r);
	ball2.x = x + r;
	ball2.y = h - (y - r);
	if (start||t!=0) {
		CPaintDC dc(this);
		CDC DrawDC;
		CBitmap Draw;
		

		DrawDC.CreateCompatibleDC(&dc);
		// 创建背景Bitmap
		Draw.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		// 将Bitmap选入DC
		CBitmap* pOldBmp = DrawDC.SelectObject(&Draw); // 记得选入
		DrawDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 255, 255));
		hBrush = CreateSolidBrush(RGB(125, min(255, int(v * 2)), 255));//画刷颜色根据速度变化
		DrawDC.SelectObject(hBrush); // 选入画刷
		DrawDC.Ellipse(ball1.x, ball1.y, ball2.x, ball2.y); // 画小球
		hBrush = CreateSolidBrush(Colors[count % 7]);//障碍物颜色
		DrawDC.SelectObject(hBrush); // 选入画刷
		DrawDC.Ellipse(Ball1.x, Ball1.y, Ball2.x, Ball2.y); // 画障碍

		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &DrawDC, 0, 0, SRCCOPY);
		DrawDC.SelectObject(pOldBmp); // 恢复原来的Bitmap
		DeleteObject(hBrush); // 删除画刷
	}
	else
	{
		CPaintDC dc(this);
		CBitmap* pOldBmp = MemDC.SelectObject(&Background); // 
		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBmp); // 恢复

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsimulateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CsimulateDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();//退出对话框
}

void CsimulateDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (start == FALSE)
	{
		//初始化尺寸
		r = n_D1 / 2;
		R = n_D2 / 2;
		//初始化位置
		w = rc.Width();
		h = rc.Height();
		X0 = w / 2;
		Y0 = h / 2;
		x = w - r;
		y = r;
		//计算障碍坐标,在这里计算是因为每次障碍位置不变，不必每次都算
		Ball1.x = X0 - R;
		Ball1.y = h - (Y0 + R);
		Ball2.x = X0 + R;
		Ball2.y = h-(Y0 - R);
		//初始化速度
		vx = -n_v * cos(n_angle * PI / 180.0)*5;
		vy = n_v * sin(n_angle * PI / 180.0)*5;
		// 初始化加速度
		ax = -n_miu*5* vx / n_v;
		ay = -n_miu * 5 * vy / n_v - 1.0; // 同时加入重力
		//转换开关
		start = TRUE;
		//初始化时间
		t = 0;
		//初始化碰撞次数
		count = 0;
	}
}











void CsimulateDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	n_angle = 15;
	Changeedit1();
}

void CsimulateDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	n_angle = 30;
	Changeedit1();
}

void CsimulateDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	n_angle = 45;
	Changeedit1();
}


void CsimulateDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	n_angle = 60;
	Changeedit1();
}

void CsimulateDlg::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	n_angle = 75;
	Changeedit1();
}


void CsimulateDlg::Changeedit1()
{
	// TODO: 在此处添加实现代码.
	CString edit1str;
	edit1str.Format(_T("你选择的发射角度是 %d °"), n_angle);
	edit1.SetWindowTextW(edit1str);
}

void CsimulateDlg::Changeedit2()
{
	// TODO: 在此处添加实现代码.
	CString edit1str;
	edit1str.Format(_T("%d"), n_D1);
	edit2.SetWindowTextW(edit1str);
}
void CsimulateDlg::Changeedit3()
{
	// TODO: 在此处添加实现代码.
	CString edit1str;
	edit1str.Format(_T("%d"), n_D2);
	edit3.SetWindowTextW(edit1str);
}




void CsimulateDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pScrollBar == &bar_d1)
	{
		switch (nSBCode)
		{
			if (pScrollBar == &bar_d1)

		case SB_THUMBTRACK:		//拖动滚动滑块时
			bar_d1.SetScrollPos(nPos);
			n_D1 = bar_d1.GetScrollPos();
			Changeedit2();
			break;
		case SB_LINEDOWN:		//单击滚动条向下的箭头
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 + 1;
			if (n_D1 > 100)
				n_D1 = 100;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		case SB_LINEUP:			//单击滚动条向上的箭头
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 - 1;
			if (n_D1 < 40)
				n_D1 = 40;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		case SB_PAGEDOWN:
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 + 3;
			if (n_D1 > 100)
				n_D1 = 100;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		case SB_PAGEUP:	//单击滚动条上面的箭头与滚动块之间的区域
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 - 3;
			if (n_D1 < 40)
				n_D1 = 40;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		}
	}
	if (pScrollBar->GetDlgCtrlID() == IDC_SLD_D2)
	{
		CSliderCtrl* pSlide = (CSliderCtrl*)pScrollBar;
		n_D2 = pSlide->GetPos();
		Changeedit3();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CsimulateDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pScrollBar->GetDlgCtrlID() == IDC_SPIN1)
	{
		n_v = nPos;
		CString str; //定义一字符串对象存储编辑框中要显示的内容
		str.Format(L"%3d", (long)n_v);
		((CSpinButtonCtrl*)pScrollBar)->GetBuddy()->SetWindowText(str);
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);

}

void CsimulateDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_cb.GetCurSel();
	CString str;
	m_cb.GetLBText(sel, str); // 获取字符串
	if (str == L"无空气阻力")n_miu = 0;
	else n_miu = _tstof(str);
}


void CsimulateDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (start)
	{
		//时间计算
		t += 1;
		//位移速度计算
		x = x + vx + 0.5 * ax;
		y = y + vy + 0.5 * ay;
		vx = vx + ax;
		vy = vy + ay;
		//边框碰撞检测处理
		if (x < r )
		{
			vx = -vx*k;
			x = r;
		}
		if (x > w - r)
		{
			vx = -vx * k;
			x = w - r;
		}
		if (y < r)
		{
			vy = -vy * k;
			y = r;
		}
		if (y > h - r)
		{
			vy = -vy * k;
			y = h - r;
		}
		//绝对速度计算
		v = sqrt(vx * vx + vy * vy);
		
		//障碍碰撞检测处理
		if ((x - X0) * (x - X0) + (y - Y0) * (y - Y0) < (r + R) * (r + R))
		{
			double angle1 = atan2(y - Y0, x - X0);
			double angle2 = atan2(-vy, -vx);
			double angle3 = angle2 - angle1;
			double anglen = angle1 - angle3;
			vx = v * cos(anglen) * k;
			vy = v * sin(anglen) * k;
			x = X0 + (r + R) * cos(angle1);
			y = Y0 + (r + R) * sin(angle1);
			count += 1;
		}
		
		//算加速度
		double friction = n_miu*5/2 ; 
		
		
		if (v != 0) {
			ax = -friction * vx / v;
			ay = -friction * vy / v - g; // 同时加入重力
		}
		else {
			ax = 0;
			ay = -g;
		}
	
		if (g!=0&&v < 0.3 && (y <= r + 5)) {
			vx = 0;
			vy = 0;
			ax = 0;
			ay = 0;
			v = 0;
			y = r;         // 确保贴地
			start = FALSE; // 停止模拟
		}
		else if (g == 0 && v < 0.1 )
		{
			vx = 0;
			vy = 0;
			ax = 0;
			ay = 0;
			v = 0;
			start = FALSE; // 停止模拟
		}
		else if (30 * t > 10000)
		{
			vx = 0;
			vy = 0;
			ax = 0;
			ay = 0;
			v = 0;
			start = FALSE; // 停止模拟
		}
		
		InvalidateRect(rc, FALSE); // 触发 OnPaint，更新绘图
		
		//更新监视器
		int time = t * 30 / 1000;
		CString tstart, tv, tvx, tvy, ta, tax, tay, tposition;
		tstart.Format(_T("模拟已运行%d秒"), time);
		tv.Format(_T("%.2f"), v);
		tvx.Format(_T("%.2f"), vx);
		tvy.Format(_T("%.2f"), vy);
		ta.Format(_T("%.2f"), sqrt(ax * ax + ay * ay));
		tax.Format(_T("%.2f"), ax);
		tay.Format(_T("%.2f"), ay);
		tposition.Format(_T("(%.0f，%.0f)"), x, y);
		t_start.SetWindowTextW(tstart);
		t_v.SetWindowTextW(tv);
		t_vx.SetWindowTextW(tvx);
		t_vy.SetWindowTextW(tvy);
		t_a.SetWindowTextW(ta);
		t_ax.SetWindowTextW(tax);
		t_ay.SetWindowTextW(tay);
		t_position.SetWindowTextW(tposition);
		//如果模拟停止，弹窗提示
		if (start == FALSE)
		{
			CString str;
			str.Format(_T("模拟结束！最终位置：(%.0f，%.0f)"),  x, y);
			AfxMessageBox(str);
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CsimulateDlg::OnBnClickedCheckNoelost()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_chk1.GetCheck())
	{
		m_chk1.SetCheck(0);
		k=0.95;
	}
	else
	{
		m_chk1.SetCheck(1);
		k= 1;
	}
}


void CsimulateDlg::OnBnClickedCheckKeyctrl()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_chk_key.GetCheck())
	{
		m_chk_key.SetCheck(0);
		key = 0;
	}
	else
	{
		m_chk_key.SetCheck(1);
		key = 1;
	}
}

void CsimulateDlg::OnBnClickedCheckNog()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_chk_nog.GetCheck())
	{
		m_chk_nog.SetCheck(0);
		g = 1.0;
	}
	else
	{
		m_chk_nog.SetCheck(1);
		g = 0;
	}
}
