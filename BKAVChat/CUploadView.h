#pragma once
#include "afxdialogex.h"
#include "CHorizontalImageList.h" 

// UploadView dialog

class CUploadView : public CDialogEx
{
	DECLARE_DYNAMIC(CUploadView)

public:
	CUploadView(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUploadView();
	afx_msg BOOL OnInitDialog(); 

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPLOADVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CHorizontalImageList m_list; 
	DECLARE_MESSAGE_MAP()
};
