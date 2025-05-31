// Cvideo.cpp: 实现文件
//

#include "pch.h"
#include "simulate.h"
#include "afxdialogex.h"
#include "Cvideo.h"


// Cvideo 对话框

IMPLEMENT_DYNAMIC(Cvideo, CDialogEx)

Cvideo::Cvideo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AX, pParent)
{

}

Cvideo::~Cvideo()
{
}

void Cvideo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_mediaplay, m_mediaPlay);
}


BEGIN_MESSAGE_MAP(Cvideo, CDialogEx)
END_MESSAGE_MAP()


// Cvideo 消息处理程序
BEGIN_EVENTSINK_MAP(Cvideo, CDialogEx)
	ON_EVENT(Cvideo, ID_mediaplay, 6505, Cvideo::Clickmediaplay, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void Cvideo::Clickmediaplay(short nButton, short nShiftState, long fX, long fY)
{
	// TODO: 在此处添加消息处理程序代码
	m_mediaPlay.put_URL(L"file:///D:/learn/VC/大作业/simulate/x64/Debug/开发过程片段.mp4");
}
