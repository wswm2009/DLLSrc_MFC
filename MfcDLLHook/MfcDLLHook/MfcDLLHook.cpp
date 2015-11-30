// MfcDLLHook.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MfcDLLHook.h"
#include "MainDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CMfcDLLHookApp

BEGIN_MESSAGE_MAP(CMfcDLLHookApp, CWinApp)
END_MESSAGE_MAP()


// CMfcDLLHookApp construction

CMfcDLLHookApp::CMfcDLLHookApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMfcDLLHookApp object

CMfcDLLHookApp theApp;


// CMfcDLLHookApp initialization
HINSTANCE ghDllInstance;
DWORD   gDlgWgThreadID;//Wg窗口的线程ID
//打印输出
void LogOut(LPCSTR lpFormat, ...)
{
	const DWORD BufSize = 2048;
	char szMsg[BufSize];

	char* pTemp = szMsg;

	va_list args; //格式化消息

	va_start(args, lpFormat);    
	vsprintf(szMsg,  lpFormat, args);  //vsprintf_s BufSize - strlen(szMsg),
	va_end(args);  

	::OutputDebugString(szMsg);
	//输出有文件名及行号的消息
}
DWORD WINAPI PorcDllAttch_DlgWg(PVOID pArg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());   //MFC DLL使用对话框资源时  都得加上这句
	CMainDlg *pDlg=new CMainDlg;
	pDlg->Create(IDD_MAINDLG);
	pDlg->ShowWindow(SW_SHOWNORMAL);
	//((CMfcDllApp *)pArg)->m_pMainWnd=(CWnd*)pDlg;
	//((CMfcDllApp *)pArg)->Run();

	//这是新开的线程执行函数 要想显示上面创建的对话框 就必须加上下面的消息loop
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 1;
}

BOOL CMfcDLLHookApp::InitInstance()
{
	CWinApp::InitInstance();


	ghDllInstance= this->m_hInstance;
	//::CloseHandle(::CreateThread(NULL, 0, PorcDllAttch_DlgWg, this, 0, &gDlgWgThreadID));
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMainDlg *pDlg=new CMainDlg;
	pDlg->Create(IDD_MAINDLG);
	pDlg->ShowWindow(SW_SHOWNORMAL);
	this->m_pMainWnd=(CWnd*)pDlg;
	this->Run();
	return TRUE;
}
