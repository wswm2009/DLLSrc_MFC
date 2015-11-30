// MfcDLLHook.h : main header file for the MfcDLLHook DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMfcDLLHookApp
// See MfcDLLHook.cpp for the implementation of this class
//

class CMfcDLLHookApp : public CWinApp
{
public:
	CMfcDLLHookApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
