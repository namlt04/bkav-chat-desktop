// UploadView.cpp : implementation file
//

#include "pch.h"
#include "BKAVChat.h"
#include "afxdialogex.h"
#include "CUploadView.h"

// UploadView dialog

IMPLEMENT_DYNAMIC(CUploadView, CDialogEx)

CUploadView::CUploadView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UPLOADVIEW, pParent)
{

}

CUploadView::~CUploadView()
{
}

void CUploadView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}
BOOL CUploadView::OnInitDialog()
{
	CDialogEx::OnInitDialog(); 
	CRect client; 
	GetClientRect(&client);

	m_list.Create(CRect(10, 10, 500, 150), this, 1234);

	/*m_list.Create(NULL, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL,
		CRect(10, 10, 400, 100), this, 1234);*/
	//m_list.AddImage(_T("C:\\Users\\thanhnam\\Downloads"));
	//m_list.AddImage(_T("C:\\Users\\thanhnam\\Downloads"));
	return TRUE;
}


BEGIN_MESSAGE_MAP(CUploadView, CDialogEx)
END_MESSAGE_MAP()


// UploadView message handlers
