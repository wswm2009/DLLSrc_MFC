// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MfcDLLHook.h"
#include "MainDlg.h"


extern CMfcDLLHookApp theApp;
extern HINSTANCE ghDllInstance;
HWND gMainWnd;
HHOOK g_hhook;
HWND hDlg;
// CMainDlg dialog

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
END_MESSAGE_MAP()


// CMainDlg message handlers

BOOL CALLBACK MyEnumProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	LPWNDINFO pInfo = (LPWNDINFO)lParam;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	if( dwProcessId == pInfo->dwProcessId )
		//确认找到的句柄不是DLL窗口的句柄
		//如果找到的是DLL窗口的句柄则返回FALSE以便继续查找宿主的窗口句柄
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}
//通过枚举窗口获取主窗口句柄
HWND GetMainWndByPID1(DWORD dwProcessId)
//将DLL窗口的句柄传入，以便识别找到的句柄不是DLL窗口的句柄
{
	WNDINFO wi={0};
	HWND re_hwnd=NULL;
	wi.dwProcessId = dwProcessId;
	wi.hWnd = NULL;

	EnumWindows(MyEnumProc,(LPARAM)&wi);
	re_hwnd= wi.hWnd;
	while( GetParent(re_hwnd) != NULL )
		//循环查找父窗口，以便保证返回的句柄是最顶层的窗口句柄
	{
		re_hwnd = GetParent( re_hwnd );
	}
	return re_hwnd;
}
LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam, LPARAM lParam)
{
	//按F12弹起时呼出外挂
	if ((lParam & (1<<31)) && code == HC_ACTION && wParam == VK_HOME) 
	{

		ShowWindow(hDlg,IsWindowVisible(hDlg)?SW_HIDE:SW_SHOW);

	}
	return CallNextHookEx(g_hhook, code, wParam ,lParam);
}
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	hDlg=this->m_hWnd;
	//gMainWnd=::FindWindow(NULL,"AppTst");
	gMainWnd=GetMainWndByPID1(GetCurrentProcessId());
	DWORD tid=::GetWindowThreadProcessId(gMainWnd,NULL);//获取线程ID
	g_hhook=::SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,ghDllInstance,tid);
	return TRUE;
}