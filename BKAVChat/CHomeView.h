#pragma once
#include "afxdialogex.h"
#include "CListFriend.h"
#include "CChatView.h"
#include "FileManager.h"
#include "GlobalParam.h"
#include "APIHelper.h"
#include "WindowMessageCustome.h"
#include "MessageHelper.h"
#include "CalcHelper.h"
#include "CCButton.h"
// CHomeView dialog

class CHomeView : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeView)

public:
	CHomeView(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHomeView();
	virtual afx_msg BOOL OnInitDialog();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOMEVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelChangeListFriend();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); 
	afx_msg LRESULT OnResponse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT LateInitData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();

	afx_msg void OnClose();
	void InitUI(); 
	CEdit m_searchBar; 
	CStatic m_infoBar;
	CCButton m_cbSearch; 
	HICON m_hiSearch; 
	HICON m_hIcon; 
	CListFriend listFriend;
	
	CFont m_fontLogo; 
	CFont m_fontText; 
	CFont m_fontTextInput; 

	CStatic m_logo; 
	
	DECLARE_MESSAGE_MAP()
};
