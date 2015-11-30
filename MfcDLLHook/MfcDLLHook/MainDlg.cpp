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
		//ȷ���ҵ��ľ������DLL���ڵľ��
		//����ҵ�����DLL���ڵľ���򷵻�FALSE�Ա�������������Ĵ��ھ��
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}
//ͨ��ö�ٴ��ڻ�ȡ�����ھ��
HWND GetMainWndByPID1(DWORD dwProcessId)
//��DLL���ڵľ�����룬�Ա�ʶ���ҵ��ľ������DLL���ڵľ��
{
	WNDINFO wi={0};
	HWND re_hwnd=NULL;
	wi.dwProcessId = dwProcessId;
	wi.hWnd = NULL;

	EnumWindows(MyEnumProc,(LPARAM)&wi);
	re_hwnd= wi.hWnd;
	while( GetParent(re_hwnd) != NULL )
		//ѭ�����Ҹ����ڣ��Ա㱣֤���صľ�������Ĵ��ھ��
	{
		re_hwnd = GetParent( re_hwnd );
	}
	return re_hwnd;
}
LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam, LPARAM lParam)
{
	//��F12����ʱ�������
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
	DWORD tid=::GetWindowThreadProcessId(gMainWnd,NULL);//��ȡ�߳�ID
	g_hhook=::SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,ghDllInstance,tid);
	return TRUE;
}