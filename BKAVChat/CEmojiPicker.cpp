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
	emoji = _T("😀 😃 😄 😁 😆 😅 😂 🤣 😊 😇 🙂 🙃 😉 😌 😍 🥰 😘 😗 😙 😚 😋 😛 😝 😜 🤪 🤨 🧐 🤓 😎 🤩 🥳 😏 😒 😞 😔 😟️");
	m_pParent = (CChatView*)pParent;
	m_vButton = std::vector<CCButton>(36);
	m_hIcon = AfxGetApp()->LoadIconW(IDI_BKAV);
	CString token;
	int pos = 0; 
	while (!(token = emoji.Tokenize(L" ", pos)).IsEmpty())
	{
		m_vIcons.push_back(token); 
	}
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
	CRect rF(0, 0, 480, 480);
	SetIcon(m_hIcon, TRUE); 
	SetIcon(m_hIcon, FALSE); 
	AdjustWindowRect(&rF, GetStyle(), FALSE);

	this->SetWindowPos(NULL, 0, 0, rF.Width(), rF.Height(), 0);
	CRect rClient;
	GetClientRect(&rClient);

	UINT cnt = 0;

	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
		{
			CRect rTmp(i * 80, j * 80, (i + 1) * 80, (j + 1) * 80);
			m_vButton[cnt].Create(m_vIcons[cnt], nullptr, rTmp, this, 9000 + cnt, 3);
			cnt += 1;
		}
	return TRUE;

}


void CEmojiPicker::OnBnClicked(UINT nId)

{
	UINT nIndex = nId%9000;
	m_pParent->EmojiHandle(m_vIcons[nIndex]);

}
BEGIN_MESSAGE_MAP(CEmojiPicker, CDialogEx)
	ON_COMMAND_RANGE(9000, 9035, &CEmojiPicker::OnBnClicked)
END_MESSAGE_MAP()


// CEmojiPicker message handlers
