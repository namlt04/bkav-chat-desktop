
// BKAVChatDlg.h : header file
//

#pragma once
#include "GlobalParam.h"
#include "WindowMessageCustome.h"
#include "json.hpp"
#include "APIHelper.h"
#include "CalcHelper.h"
#include "CCButton.h"

// CBKAVChatDlg dialog
class CLoginView : public CDialogEx
{
// Construction
public:
	CLoginView(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGINVIEW };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CCButton sign_in_button;
	CCButton sign_up_button;
	CStatic usernameEmptyError;
	CStatic passwordEmptyError;
	CStatic usernamePasswordInvalid;
	CStatic m_logo; 
	CStatic m_textAccount; 
	CStatic m_textPassword;
	CEdit m_editAccount; 
	CEdit m_editPassword; 
	CButton m_cbRemember;

	CString m_usernameController;
	CString m_passwordController;
	BOOL m_checkRemember;

	CFont m_logoFont; 
	CFont m_textFont;
	CFont m_textFontInput; 

	CBrush* m_backgroundBrush = CBrush::FromHandle(::GetSysColorBrush(COLOR_BTNFACE));
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nColor);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void SignUpButtonClicked();
	afx_msg void SignInButtonClicked();
	afx_msg LRESULT OnResponse(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnResponseFriend(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck();
};
