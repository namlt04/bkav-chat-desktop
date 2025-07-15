
// BKAVChat.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "CHomeView.h"
#include "CLoginView.h"
#include "DatabaseManager.h"


// CBKAVChatApp:
// See BKAVChat.cpp for the implementation of this class
//

class CBKAVChatApp : public CWinApp
{
public:
	CBKAVChatApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBKAVChatApp theApp;
