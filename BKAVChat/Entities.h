#pragma once
#include <afxstr.h>

namespace Entities
{
	struct User 
	{
		CString showName; 
		CString fullname = _T("");
		CString username; 
		CString content; 
		CString friendId;
		CString avatar = _T("");
		HICON hIcon;
		UINT isOnline = 0;
	};

	struct Message
	{
		CString friendId;
		UINT type; 
		CString content;
		CString link; 
		CString time; 
		UINT width = 0; 
		UINT height = 0; 
		UINT messageType; 
	};
}
