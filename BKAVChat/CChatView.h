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
#include "CListMessage.h"
#include "CFilePicker.h"
// CChatView dialog

class CChatView : public CDialogEx
{
	DECLARE_DYNAMIC(CChatView)

public:
	CChatView(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CChatView();
	void ReceivedData(Entities::User userReceived); 
	void EmojiHandle(CString emoji);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATVIEW };
#endif


 protected:
	  
	afx_msg LRESULT CloseFilePicker(WPARAM wParam, LPARAM lParam);
	 //LRESULT UpdateScreen(WPARAM wParam, LPARAM lParam); 
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK(); 
	afx_msg void OnSendButtonClicked();
	afx_msg LRESULT OnResponseGetAll(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnResponseGetLast(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnEmojiButtonClicked();

	afx_msg void OnImageButtonClicked();
	afx_msg void OnFileButtonClicked();
	afx_msg void OnPaint(); 
	afx_msg BOOL CChatView::OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnResponseSend(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnClickDownloadFile(WPARAM wParam, LPARAM lParam);
	void SaveMessageIntoCache(std::vector<Entities::Message> vt, bool isCreate); 
	void AddItemToListMessage(std::vector<Entities::Message>& vt); 
	CRect rInput; // input + send; 
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
	CFilePicker m_list; 
	CEmojiPicker* pEmojiPicker;


	HICON m_hSend; 
	HICON m_hPhoto;
	HICON m_hAttach; 
	HICON m_hEmoji; 
	DECLARE_MESSAGE_MAP()
};
