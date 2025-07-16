// CEmojiPicker.cpp : implementation file
//

#include "pch.h"
#include "BKAVChat.h"
#include "afxdialogex.h"
#include "CEmojiPicker.h"

// CEmojiPicker dialog

IMPLEMENT_DYNAMIC(CEmojiPicker, CDialogEx)

CEmojiPicker::CEmojiPicker(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EMOJIPICKER, pParent)
{
	m_pParent = (CChatView*) pParent;
	m_vbutton = std::vector<CCButton>(10); 
	m_hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME); 

}

CEmojiPicker::~CEmojiPicker()
{
}

void CEmojiPicker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BOOL CEmojiPicker::OnInitDialog()
{
	CDialogEx::OnInitDialog(); 
	this->SetWindowTextW(_T("BKAV Chat - Emoji")); 
	this->SetWindowPos(NULL, 0, 0, 400, 200, 0);
	CRect rClient;
	GetClientRect(&rClient); 
	for (int i = 0; i < 10; i++)

	{

		CRect rect(i * 40, 0 , (i + 1) * 40, 40);
		m_vbutton[i].Create(&m_hIcon, rect, this, 1000 + i + 1); 
	}
	return TRUE;
	
}


void CEmojiPicker::OnBnClicked(UINT nId) 

{
	UINT nIndex = 1011 - nId;
	m_pParent->EmojiHandle(nIndex);

}
BEGIN_MESSAGE_MAP(CEmojiPicker, CDialogEx)
	ON_COMMAND_RANGE(1001, 1011, &CEmojiPicker::OnBnClicked) 
END_MESSAGE_MAP()


// CEmojiPicker message handlers
