#pragma once
#include "afxdialogex.h"
#include "WindowMessageCustome.h"
#include "APIHelper.h"
#include "json.hpp"
#include "CalcHelper.h" 
#include "CCButton.h"

// CRegisterView dialog

class CRegisterView : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterView)

public:
	CRegisterView(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CRegisterView();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTERVIEW };
#endif


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void SignUpButtonClicked();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnResponse(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDc, CWnd* pWnd, UINT nColor);

	CCButton sign_up_button;
	HICON m_hIcon; 
	CFont m_fontLogo; 
	CFont m_fontText; 
	CFont m_fontTextInput;


	CStatic m_textLogo; 


	CStatic m_textFullname; 
	CStatic m_textAccount; 
	CStatic m_textPassword; 
	CStatic m_textRePassword; 

	CEdit m_editFullname; 
	CEdit m_editAccount; 
	CEdit m_editPassword; 
	CEdit m_editRePassword; 


	CStatic accountExist; 
	CStatic passwordMissmatch;
	CStatic registerSuccessfully;
	
	CString m_fullnameController;
	CString m_usernameController;
	CString m_passwordController;
	CString m_repasswordController;
	DECLARE_MESSAGE_MAP()
};
