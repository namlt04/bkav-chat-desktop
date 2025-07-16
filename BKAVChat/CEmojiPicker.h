#pragma once
#include "afxdialogex.h"
#include "CCButton.h"
#include <vector>
// CEmojiPicker dialog

//#include "CChatView.h"
class CChatView;
class CEmojiPicker : public CDialogEx
{
	DECLARE_DYNAMIC(CEmojiPicker)

public:
	CEmojiPicker(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEmojiPicker();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMOJIPICKER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(); 
	afx_msg void OnBnClicked(UINT nId);
	CChatView* m_pParent = nullptr; 
	std::vector<CCButton> m_vButton; 
	HICON m_hIcon; 
	DECLARE_MESSAGE_MAP()
};
