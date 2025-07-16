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
	m_vButton = std::vector<CCButton>(16); 
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
	CRect rF(0, 0, 160, 160); 
	AdjustWindowRect(&rF, GetStyle(), FALSE); 

	this->SetWindowPos(NULL, 0, 0, rF.Width(), rF.Height(), 0);
	CRect rClient;
	GetClientRect(&rClient); 
	
	UINT cnt = 0; 
	
	while (cnt < 16)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				CRect rTmp(i * 40, j * 40, (i + 1) * 40,(j + 1)*40);
				m_vButton[cnt].Create(&m_hIcon, rTmp, this, 1000 + cnt);
				cnt += 1; 
			}
	}
	return TRUE;
	
}


void CEmojiPicker::OnBnClicked(UINT nId) 

{
	UINT nIndex = 1015 - nId;
	m_pParent->EmojiHandle(nIndex);

}
BEGIN_MESSAGE_MAP(CEmojiPicker, CDialogEx)
	ON_COMMAND_RANGE(1000, 1015, &CEmojiPicker::OnBnClicked) 
END_MESSAGE_MAP()


// CEmojiPicker message handlers
