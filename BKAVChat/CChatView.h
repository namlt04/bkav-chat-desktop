#pragma once
#include "afxdialogex.h"
#include "CListMessage.h"
#include "WindowMessageCustome.h"
#include "APIHelper.h"
#include "MessageHelper.h"
#include "GlobalParam.h"
#include "Entities.h"
#include <vector>
#include "CEmojiPicker.h"
#include "CCButton.h"
// CChatView dialog

class CChatView : public CDialogEx
{
	DECLARE_DYNAMIC(CChatView)

public:
	CChatView(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CChatView();
	void ReceivedData(Entities::User userReceived); 
	void EmojiHandle(UINT sel);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK(); 
	afx_msg void OnSendButtonClicked();
	afx_msg LRESULT OnResponseGetAll(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnResponseGetLast(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnEmojiButtonClicked();

	afx_msg void OnImageButtonClicked();
	afx_msg void OnFileButtonClicked();
	afx_msg void OnSelChange();
	afx_msg LRESULT OnResponseSend(WPARAM wParam, LPARAM lParam); 
	void SaveMessageIntoCache(std::vector<Entities::Message> vt, bool isCreate); 
	void AddItemToListMessage(std::vector<Entities::Message>& vt); 
	Entities::User user; 
	CListMessage listMessage;

	CEdit input;
	CFont m_font;
	CFont m_fontDownload; 
	HICON m_hIcon; 
	CString m_inputController;
	CCButton m_sendButton;
	CCButton m_emojiButton; 
	CCButton m_imageButton; 
	CCButton m_fileButton;
	
	CEmojiPicker* m_pEmojiPicker;
	DECLARE_MESSAGE_MAP()
};
