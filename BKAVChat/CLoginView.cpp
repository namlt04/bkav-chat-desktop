
// BKAVChatDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BKAVChat.h"
#include "CLoginView.h"
#include "afxdialogex.h"
#include "CRegisterView.h"
#include "CHomeView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBKAVChatDlg dialog


CLoginView::CLoginView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGINVIEW, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, 1103, m_usernameController);
	//DDX_Text(pDX, 1105, m_passwordController);
	//DDX_Check(pDX , 1106, m_checkRemember);
}

BEGIN_MESSAGE_MAP(CLoginView, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(1108, &CLoginView::SignUpButtonClicked)
	ON_BN_CLICKED(1107, &CLoginView::SignInButtonClicked)
	ON_MESSAGE(WM_API_LOGIN, &CLoginView::OnResponse)

END_MESSAGE_MAP()


// CBKAVChatDlg message handlers

BOOL CLoginView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	this->SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER);
	this->SetWindowTextW(_T("BKAV Chat - Đăng nhập")); 
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//m_brushBackground.CreateSolidBrush(RGB(255, 255, 255)); 

	CRect rClient;
	GetClientRect(&rClient);
	UINT width = rClient.Width();
	UINT height = rClient.Height();
	m_logoFont.CreateFont(
		height / 10,                    // Chiều cao font (pixel)
		0,                     // Chiều rộng (0 = tự tính)
		0, 0,                  // Escapement, Orientation
		FW_BOLD,               // Đậm
		FALSE,                 // Italic
		FALSE,                 // Underline
		0,                     // Strikeout
		ANSI_CHARSET,          // Charset (dùng ANSI nếu không cần Unicode đặc biệt)
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		_T("Segoe UI")         // Tên font
	);
	m_textFont.CreateFont(
		height / 15,                    // Chiều cao font (pixel)
		0,                     // Chiều rộng (0 = tự tính)
		0, 0,                  // Escapement, Orientation
		FW_NORMAL,               // Đậm
		FALSE,                 // Italic
		FALSE,                 // Underline
		0,                     // Strikeout
		ANSI_CHARSET,          // Charset (dùng ANSI nếu không cần Unicode đặc biệt)
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		_T("Segoe UI")         // Tên font
	);


	m_textFontInput.CreateFont(
		height / 18,                    // Chiều cao font (pixel)
		0,                     // Chiều rộng (0 = tự tính)
		0, 0,                  // Escapement, Orientation
		FW_NORMAL,               // Đậm
		FALSE,                 // Italic
		FALSE,                 // Underline
		0,                     // Strikeout
		ANSI_CHARSET,          // Charset (dùng ANSI nếu không cần Unicode đặc biệt)
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		_T("Segoe UI")         // Tên font
	);
	CClientDC dc(this);
	

	CRect rSL = CalcHelper::CalcTextRect(&dc, _T("BKAV Chat"), &m_logoFont); // rect static text logo 
	m_logo.Create(_T("BKAV Chat"), WS_CHILD | WS_VISIBLE, rSL, this, 1101);
	m_logo.SetFont(&m_logoFont, true); 




	CRect rSA = CalcHelper::CalcTextRect(&dc, _T("Tài khoản"), &m_textFont); // rect static text account
	UINT tmpH = rSA.Height(); 
	UINT tmpW = rSA.Width(); 
	rSA.top =  (height / 5); 
	rSA.bottom = rSA.top + tmpH; 
	rSA.left = width / 5; 
	rSA.right = rSA.left + tmpW; 
	m_textAccount.Create(_T("Tài khoản"), WS_CHILD | WS_VISIBLE, rSA, this, 1102);
	m_textAccount.SetFont(&m_textFont, true);

	CRect rTA((width / 2 +  rSA.left) / 2, rSA.top, 4 * (width / 5), rSA.top + rSA.Height()); // rect text edit account

	m_editAccount.Create(WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER , rTA, this, 1103);
	m_editAccount.SetFont(&m_textFontInput, true); 

	CRect rSP = CalcHelper::CalcTextRect(&dc, _T("Mật khẩu"), &m_textFont) ; // rect static text password
	tmpH = rSP.Height(); 
	tmpW = rSP.Width();
	rSP.top = 2 * (height / 5) - (height / 15); 
	rSP.bottom = rSP.top + tmpH; 
	rSP.left = rSA.left; 
	rSP.right = rSP.left + tmpW; 

	m_textPassword.Create(_T("Mật khẩu"), WS_CHILD | WS_VISIBLE, rSP, this, 1104); 
	m_textPassword.SetFont(&m_textFont, true); 
	CRect rTP(rTA.left, rSP.top, 4 * (width /5), rSP.top + rSP.Height());  // rect text edit password
	m_editPassword.Create(WS_CHILD | WS_VISIBLE | ES_PASSWORD | ES_LEFT | WS_BORDER, rTP, this, 1105); 


	
	CRect rBR = CalcHelper::CalcTextRect(&dc, _T("Nhớ tài khoản và mật khẩu"), &m_textFont); 
	tmpH = rBR.Height(); 
	tmpW = rBR.Width() +  (width / 30); 
	rBR.left = (width - tmpW) / 2; 
	rBR.right = rBR.left + tmpW;
	rBR.top = rSP.bottom + height / 12; 
	rBR.bottom = rBR.top + tmpH; 
	m_cbRemember.Create(_T("Nhớ tài khoản và mật khẩu"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, rBR, this,1106 ); 
	m_cbRemember.SetFont(&m_textFont); 


	// Xong Button
	UINT widthButton = static_cast<UINT> (width /4); 
	UINT heightButton = static_cast<UINT> ((width / 14) ); 

	CRect rSib; 
	rSib.SetRectEmpty(); 
	rSib.left = (width - widthButton) / 2; // rect sign in button
	rSib.top = (height * 3) / 5; // rect sign in button
	rSib.right = rSib.left + widthButton;  // rect sign in button
	rSib.bottom = rSib.top + heightButton; // rect sign in button

	sign_in_button.Create(_T("Đăng nhập"), rSib, this, 1107, &m_textFont, true); 

	CRect rSub(rSib.left, rSib.bottom + (heightButton / 2), rSib.left + widthButton, rSib.bottom + ((3 * heightButton)/2));

	sign_up_button.Create(_T("Đăng ký"),  rSub, this, 1108, &m_textFont,false);

	//sign_up_button.SetFaceColor(GetSysColor(COLOR_3DFACE), FALSE); // Nền trùng với nền dialog (xóa hiệu ứng)
	//sign_up_button.SetTextColor(RGB(0, 0, 0)); // Chữ màu đen
	




	// Hien thi cac thong bao loi 
	
	CRect noti_1 = CalcHelper::CalcTextRect(&dc, _T("Tên đăng nhập không được để trống"), &m_textFont);
	noti_1.MoveToXY(CPoint( ( width - noti_1.Width())/2 , 8 * (height / 9)));
	usernameEmptyError.Create(_T("Tên đăng nhập không được để trống "), WS_CHILD | WS_VISIBLE, noti_1, this, 1109);
	usernameEmptyError.SetFont(&m_textFont); 
	usernameEmptyError.ShowWindow(SW_HIDE);

	CRect noti_2 = CalcHelper::CalcTextRect(&dc, _T("Mật khẩu không được để trống"), &m_textFont);
	noti_2.MoveToXY(CPoint( ( width - noti_2.Width())/2 , 8 * (height / 9)));
	passwordEmptyError.Create(_T("Mật khẩu không được để trống"), WS_CHILD | WS_VISIBLE, noti_2, this, 1110);
	passwordEmptyError.SetFont(&m_textFont); 
	passwordEmptyError.ShowWindow(SW_HIDE);

	CRect noti_3 = CalcHelper::CalcTextRect(&dc, _T("Bạn nhập sai tên tài khoản hoặc mật khẩu!"), &m_textFont);
	noti_3.MoveToXY(CPoint( ( width - noti_3.Width())/2 , 8 * (height / 9)));
	usernamePasswordInvalid.Create(_T("Bạn nhập sai tên tài khoản hoặc mật khẩu!"), WS_CHILD | WS_VISIBLE, noti_3, this, 1111);
	usernamePasswordInvalid.SetFont(&m_textFont); 
	usernamePasswordInvalid.ShowWindow(SW_HIDE);


	CString result = FileManager::ReadConfig(); 
	CT2A temp(result);
	std::string str_result = std::string(temp);
	nlohmann::json j; 
	j = nlohmann::json::parse(str_result);
	//
	if (j["remember"])
	
	{
		std::string username = j["username"];
		m_usernameController = CString(CA2T(username.c_str()));

		std::string password = j["password"];
		m_passwordController = CString(CA2T(password.c_str()));
		m_checkRemember = true;
		m_editAccount.SetWindowText(m_usernameController);
		m_editPassword.SetWindowText(m_passwordController);
		m_cbRemember.SetCheck(m_checkRemember ? BST_CHECKED : BST_UNCHECKED);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoginView::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoginView::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoginView::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLoginView::SignUpButtonClicked() 
{

	CRegisterView dlg;
	dlg.DoModal();

}
void CLoginView::OnBnClickedCheck()
{
	m_checkRemember ? FALSE : TRUE;

}
void CLoginView::SignInButtonClicked()
{
	m_editAccount.GetWindowText(m_usernameController);
	m_editPassword.GetWindowText(m_passwordController);
	m_checkRemember = (m_cbRemember.GetCheck() == BST_CHECKED);
	if (m_usernameController == _T("") && m_passwordController == _T(""))
	{
		// Nếu cả username và password trống
		return;
	}
	if (m_usernameController == _T(""))  // Nếu username trống 
	{
		usernameEmptyError.ShowWindow(SW_SHOW);
		SetTimer(1, 3000, nullptr);
		return;
	}

	if (m_passwordController == _T("")) // Nếu password trống
	{
		passwordEmptyError.ShowWindow(SW_SHOW);
		SetTimer(2, 3000, nullptr);
		return;
	}
	APIHelper::Login(this->GetSafeHwnd(), m_usernameController, m_passwordController);
}
void CLoginView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		usernameEmptyError.ShowWindow(SW_HIDE);
		KillTimer(1);
		break;
	case 2:
		passwordEmptyError.ShowWindow(SW_HIDE);
		KillTimer(2);
		break;
	case 3:
		usernamePasswordInvalid.ShowWindow(SW_HIDE);
		KillTimer(3);
		break;
	}

}
LRESULT CLoginView::OnResponse(WPARAM wParam, LPARAM  lParam)
{
	// Chua thiêt lap UINT WM_LOGIN_RESPONSE 
	AfxMessageBox(_T("Da nhan")); 
	std::string* pResponse = reinterpret_cast<std::string*>(lParam);
	//CString* pResponse = (CString*)lParam; 
	//CT2A tmp(*pResponse);
	//std::string str = std::string(tmp);

	nlohmann::json j;
	j = nlohmann::json::parse(*pResponse);
	if (j["status"] == 1)
	{
		// Luu config
		FileManager::SaveConfig(m_usernameController, m_passwordController, m_checkRemember);
		
		// Luu token
		std::string token_str = j["data"]["token"];
		CString token(CA2T(token_str.c_str())); 
		GlobalParam::token = token;
		
		/* pCHomeView->Create(IDD_HOMEVIEW, AfxGetMainWnd());

		AfxGetApp()->m_pMainWnd = pCHomeView;
		pCHomeView->ShowWindow(SW_HIDE); */
		/* if (pCHomeView->Create(IDD_HOMEVIEW, AfxGetMainWnd())) {

			// In ra HWND dưới dạng số thập phân (hoặc hexa)
			CString str;
			str.Format(_T("HWND của pCHomeView: 0x%08X"), (UINT)(UINT_PTR)pCHomeView->GetSafeHwnd());
			AfxMessageBox(str);

			AfxGetApp()->m_pMainWnd = pCHomeView;
			pCHomeView->InitializeData(); 
			pCHomeView->ShowWindow(SW_HIDE);

		} */

		//CHomeView* pCHomeView = new CHomeView();
		//AfxGetApp()->m_pMainWnd = pCHomeView;
		//EndDialog(0);
		//pCHomeView->DoModal(); 
		CDialogEx::OnOK(); 
	}
	else
	{
		usernamePasswordInvalid.ShowWindow(SW_SHOW);
		SetTimer(3, 3000, nullptr);
	}
	return 0;

}

HBRUSH CLoginView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	UINT nId = pWnd->GetDlgCtrlID(); 
	if ( nId == 1101) // Toàn bộ nền dialog
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(28, 127, 217)); 
		//return ::GetSysColorBrush(COLOR_BTNFACE);
		//return m_brushBackground;
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	else
		if (nId == 1109 || nId == 1110 || nId == 1111) 
		{
			pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(236, 70, 34)); 
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}
