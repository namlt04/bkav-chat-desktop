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
	afx_msg LRESULT OnResponseGetAllMessages(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnResponseGetLastMessages(WPARAM wParam, LPARAM lParam); 
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
	CString m_inputController;
	CButton sendButton;
	CButton emojiButton; 
	CButton imageButton; 
	CButton fileButton;
	CEmojiPicker* m_pEmojiPicker;
	DECLARE_MESSAGE_MAP()
};
