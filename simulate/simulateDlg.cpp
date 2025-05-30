
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

#define PI 3.1415926535897   //宏定义圆周率，用于角度转换

// CsimulateDlg 对话框
//全局变量

COLORREF Colors[7] = {
	RGB(255, 0, 0),      // 红
	RGB(255, 128, 0),    // 橙
	RGB(255, 255, 0),    // 黄
	RGB(0, 255, 0),      // 绿
	RGB(0, 0, 255),      // 蓝
	RGB(75, 0, 130),     // 靛
	RGB(139, 0, 255)     // 紫
};//障碍颜色

CsimulateDlg::CsimulateDlg(CWnd* pParent /*=nullptr*/)//构造函数，自动生成，实际变量在Initial初始化，此处警告可忽略
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
	DDX_Radio(pDX, IDC_RADIO1, n_angle);//发射角度单选按钮
	DDX_Control(pDX, IDC_EDIT1, edit1);//发射角度显示
	DDX_Control(pDX, IDC_BAR_D1, bar_d1);//小球半径条
	DDX_Control(pDX, IDC_SLD_D2, sld_d2);//障碍半径条
	DDX_Control(pDX, IDC_EDIT2, edit2);//小球半径显示
	DDX_Control(pDX, IDC_EDIT3, edit3);//障碍半径显示
	DDX_Control(pDX, IDC_COMBO1, m_cb); //空气阻力下拉框
	DDX_Control(pDX, IDC_CHECK_NOELOST, m_chk1);//弹性碰撞复选框
	DDX_Control(pDX, IDC_CHECK_KEYCTRL, m_chk_key); //方向键控制复选框
	DDX_Control(pDX, IDC_CHECK_NOG, m_chk_nog); //无重力复选框
	//以下是用于显示监视数据的静态控件
	DDX_Control(pDX, IDT_START, t_start);
	DDX_Control(pDX, IDT_V, t_v);
	DDX_Control(pDX, IDT_VX, t_vx);
	DDX_Control(pDX, IDT_VY, t_vy);
	DDX_Control(pDX, IDT_A, t_a);
	DDX_Control(pDX, IDT_AX, t_ax);
	DDX_Control(pDX, IDT_AY, t_ay);
	DDX_Control(pDX, IDT_POSITION, t_position);
	DDX_Control(pDX, IDC_RADIO1, m_angle1);
	DDX_Control(pDX, IDC_RADIO2, m_angle2);
	DDX_Control(pDX, IDC_RADIO3, m_angle3);
	DDX_Control(pDX, IDC_RADIO4, m_angle4);
	DDX_Control(pDX, IDC_RADIO5, m_angle5);
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
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
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
	//初始化起始运动变量
	//经过测试，这里初始化会导致严重bug，所以改到视图中进行初始化
	// 务必初始化，否则会有更严重的bug
	//控件初始化，设置调节范围和默认值
	bar_d1.SetScrollRange(40, 100);
	bar_d1.SetScrollPos(n_D1);
	sld_d2.SetRange(50, 200);
	sld_d2.SetPos(n_D2);
	m_cb.AddString(L"0.1");
	m_cb.AddString(L"0.2");
	m_cb.AddString(L"0.3");
	m_cb.AddString(L"无空气阻力");
	//根据预设的角度和摩擦设置单选框和下拉框状态
	if (n_miu == 0.1)m_cb.SetCurSel(0);
	else if (n_miu == 0.2)m_cb.SetCurSel(1);
	else if (n_miu == 0.3)m_cb.SetCurSel(2);
	else if (n_miu==0)m_cb.SetCurSel(3);
	m_chk1.SetCheck(1); // 默认选中弹性碰撞
	if (n_angle == 15)m_angle1.SetCheck(1);
	else if (n_angle == 30)m_angle2.SetCheck(1);
	else if (n_angle == 45)m_angle3.SetCheck(1);
	else if (n_angle == 60)m_angle4.SetCheck(1);
	else if (n_angle == 75)m_angle5.SetCheck(1);
	//在编辑框控件中显示选中值（角度、两球直径）
	Changeedit1();
	Changeedit2();
	Changeedit3();
	//初速度设置（spin控件）
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	pSpin->SetRange(10, 30);
	pSpin->SetPos(n_v);
	CString nv = L"";
	nv.Format(L"%d", n_v);
	pSpin->GetBuddy()->SetWindowText(nv);
	//绘图操作
	CClientDC dc(this);
	GetDlgItem(IDC_RECT)->GetWindowRect(&rc);// 获取绘图区域的矩形的坐标，存储到rc中，这个rc就是所有绘图区域
	ScreenToClient(&rc); // 转换为对话框内部坐标

	//这里引入双缓冲绘图，绘制白色背景
	MemDC.CreateCompatibleDC(&dc);// 创建兼容DC
	Background.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());// 创建背景Bitmap
	CBitmap* pOldBmp = MemDC.SelectObject(&Background);// 将Bitmap选入DC
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 255, 255));	// 填充白色背景
	MemDC.SelectObject(pOldBmp); // 恢复原来的Bitmap
	Invalidate(); // 刷新，触发一次绘图

	//初始化定时器，30ms刷新一次
	SetTimer(1, 30, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsimulateDlg::OnPaint()
{  //计算小球绘图坐标
	ball1.x = x - r;
	ball1.y = h - (y + r);
	ball2.x = x + r;
	ball2.y = h - (y - r);
	//如果模拟已经开始且t不是0（模拟结束）
	if (start || t != 0) {
		CPaintDC dc(this);
		//创建绘图DC和Bitmap
		CDC DrawDC;
		CBitmap Draw;
		// 创建兼容dc和位图
		DrawDC.CreateCompatibleDC(&dc);
		Draw.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		CBitmap* pOldBmp = DrawDC.SelectObject(&Draw); 	// 将Bitmap选入DC
		DrawDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 255, 255)); // 填充白色背景
		hBrush = CreateSolidBrush(RGB(min(125 + v*3, 255), min(255, int(v * 2)), max(255 - 1 * v,128)));//画刷颜色根据速度变化
		DrawDC.SelectObject(hBrush); // 选入画刷
		DrawDC.Ellipse(ball1.x, ball1.y, ball2.x, ball2.y); // 画小球
		hBrush = CreateSolidBrush(Colors[count % 7]);//根据碰撞次数障碍物颜色
		DrawDC.SelectObject(hBrush); // 选入画刷
		DrawDC.Ellipse(Ball1.x, Ball1.y, Ball2.x, Ball2.y); // 画障碍
		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &DrawDC, 0, 0, SRCCOPY);//一次输出
		DrawDC.SelectObject(pOldBmp); // 恢复原来的Bitmap
		DeleteObject(hBrush); // 删除画刷
	}
	else
	{	//如果模拟未开始，仅仅绘制背景
		CPaintDC dc(this);
		CBitmap* pOldBmp = MemDC.SelectObject(&Background); 
		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBmp); 
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsimulateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//下面开始是各种控件的响应函数

//以下是两个主要按钮的响应函数
void CsimulateDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();//退出对话框
}

void CsimulateDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//点击开始模拟按钮后，如果模拟没有运行，初始化数据，开始模拟
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
		Ball2.y = h - (Y0 - R);
		//初始化速度
		vx = -n_v * cos(n_angle * PI / 180.0) * 5;
		vy = n_v * sin(n_angle * PI / 180.0) * 5;
		// 初始化加速度
		ax = -n_miu*2.5 * vx / (5*n_v) + keyax;
		ay = -n_miu*2.5 * vy / (5*n_v) - g + keyay; // 同时加入重力
		//转换开关
		start = TRUE;
		//初始化时间
		t = 0;
		//初始化碰撞次数
		count = 0;
	}
}


//以下是单选按钮组的响应函数
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

//下面是展示角度、小球直径、障碍直径的编辑框的数值显示函数，因为逻辑接近，放在一块
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

//水平滚动条（包含滑竿和滚动条，分别控制两个半径），参考课件代码
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
		case SB_LINEDOWN:		//单击滚动条向右的箭头
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 + 1;
			if (n_D1 > 100)
				n_D1 = 100;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		case SB_LINEUP:			//单击滚动条向左的箭头
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 - 1;
			if (n_D1 < 40)
				n_D1 = 40;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		case SB_PAGEDOWN://单击滚动条右面的箭头与滚动块之间的区域
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 + 3;
			if (n_D1 > 100)
				n_D1 = 100;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		case SB_PAGEUP:	//单击滚动条左面的箭头与滚动块之间的区域
			n_D1 = bar_d1.GetScrollPos();
			n_D1 = n_D1 - 3;
			if (n_D1 < 40)
				n_D1 = 40;
			bar_d1.SetScrollPos(n_D1);
			Changeedit2();
			break;
		}
	}
	if (pScrollBar->GetDlgCtrlID() == IDC_SLD_D2)//滑竿的逻辑，参考了课件
	{
		CSliderCtrl* pSlide = (CSliderCtrl*)pScrollBar;
		n_D2 = pSlide->GetPos();
		Changeedit3();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
//垂直条，实际就是spin控件
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
//选择摩擦系数的下拉框响应函数
void CsimulateDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_cb.GetCurSel();
	CString str;
	m_cb.GetLBText(sel, str); // 获取字符串
	if (str == L"无空气阻力")n_miu = 0;
	else n_miu = _tstof(str);
}

//计时器响应函数，处理运动逻辑
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
		if (x < r)
		{
			vx = -vx * k;
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
		{//如果小球与障碍碰撞，速度以法线对称（角度方向改变），然后球移出去防止反复碰撞
			double angle1 = atan2(y - Y0, x - X0);
			double angle2 = atan2(-vy, -vx);
			double angle3 = angle2 - angle1;
			double anglen = angle1 - angle3;
			vx = v * cos(anglen) * k;
			vy = v * sin(anglen) * k;
			x = X0 + (r + R) * cos(angle1);
			y = Y0 + (r + R) * sin(angle1);
			count += 1;//碰撞次数加1，改变障碍颜色
		}

		//算加速度
		double friction = n_miu * 5 / 2;
		//这里引入了一个局部的摩擦变量，主要是方便调数值
		if (v != 0) {
			ax = -friction * vx / v + keyax;
			ay = -friction * vy / v - g + keyay; //如果v不为0，加速度按速度分量分配
		}
		else {//否则只有key加速度
			ax = 0+keyax;
			ay = -g+keyay;
		}

		if (g != 0 && v < 0.3 && (y <= r + 5)) {
			vx = 0;
			vy = 0;
			ax = 0;
			ay = 0;
			v = 0;
			y = r;         // 如果靠近地面且速度非常小，停止运动，贴地
			start = FALSE; 
		}
		else if (g == 0 && v < 0.1)
		{
			vx = 0;
			vy = 0;
			ax = 0;
			ay = 0;
			v = 0;
			start = FALSE; // 如果无重力情况下速度极小，停止模拟
		}
		else if (30 * t > 10000)
		{
			vx = 0;
			vy = 0;
			ax = 0;
			ay = 0;
			v = 0;
			start = FALSE; // 如果超时，停止模拟
		}
		InvalidateRect(rc, FALSE); // 触发 OnPaint，更新绘图；只更新模拟区，防止控件闪烁。

		//更新监视器
		int time = t * 30 / 1000;
		CString tstart, tv, tvx, tvy, ta, tax, tay, tposition;
		tstart.Format(_T("模拟已运行%d秒"), time);
		tv.Format(_T("%.2f"), v/5);
		tvx.Format(_T("%.2f"), vx/5);
		tvy.Format(_T("%.2f"), vy/5);
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
		//如果模拟停止，弹窗提示，这里不会触发多次，因为只有start，才会运行这个代码
		if (start == FALSE)
		{
			CString str;
			str.Format(_T("模拟结束！最终位置：(%.0f，%.0f)"), x, y);
			AfxMessageBox(str);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
//如果勾选弹性碰撞，设置碰撞时速度无损失
void CsimulateDlg::OnBnClickedCheckNoelost()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_chk1.GetCheck())
	{
		m_chk1.SetCheck(0);
		k = 0.95;
	}
	else
	{
		m_chk1.SetCheck(1);
		k = 1;
	}
}

//根据勾选情况选择按键控制开关
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
//根据勾选情况设置重力
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
//键盘按下和松开事件处理函数，用于方向键控制小球运动，兼容了wasd和方向键
void CsimulateDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (key)
	{
		switch (nChar)
		{
		case VK_LEFT:
			keyax = -1;
			break;
		case VK_RIGHT:
			keyax = 1;
			break;
		case VK_UP:
			keyay = 1;
			break;
		case VK_DOWN:
			keyay = -1;
			break;
		case 'A':
			keyax = -1;
			break;
		case 'D':
			keyax = 1;
			break;
		case 'W':
			keyay = 1;
			break;
		case 'S':
			keyay = -1;
			break;
		default:
			break;
		}
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
//松开按键时设置按键加速度/为0
void CsimulateDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	keyax = 0;
	keyay = 0;
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}
//预处理按键消息函数
BOOL CsimulateDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN|| pMsg->message == WM_KEYUP)
	{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
