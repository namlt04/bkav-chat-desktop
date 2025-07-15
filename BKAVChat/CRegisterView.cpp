// CRegisterView.cpp : implementation file
//

#include "pch.h"
#include "BKAVChat.h"
#include "afxdialogex.h"
#include "CRegisterView.h"

// Toan bo cac controller deu bat dau bang id 1200 , controller tự tạo bằng code


// CRegisterView dialog

IMPLEMENT_DYNAMIC(CRegisterView, CDialogEx)

CRegisterView::CRegisterView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTERVIEW, pParent)
{

}

CRegisterView::~CRegisterView()
{
}

void CRegisterView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CRegisterView::SignUpButtonClicked() {
	m_editFullname.GetWindowText(m_fullnameController); 
	m_editAccount.GetWindowText(m_usernameController); 
	m_editPassword.GetWindowText(m_passwordController); 
	m_editRePassword.GetWindowText(m_repasswordController); 

	if (m_passwordController != m_repasswordController) 
	{
		passwordMissmatch.ShowWindow(SW_SHOW);
		SetTimer(2, 3000, nullptr);
	}
	else
	{
		APIHelper::Register(this->GetSafeHwnd(), m_fullnameController, m_usernameController, m_passwordController);
	}

	

}
BOOL CRegisterView::OnInitDialog() {
	CDialogEx::OnInitDialog();
	this->SetWindowTextW(_T("BKAV Chat - Đăng ký"));
	this->SetWindowPos(NULL, 0, 0, 600, 500, SWP_NOMOVE | SWP_NOZORDER);
	//CRect rectSignUpButton(200, 270, 290, 300);


	//sign_up_button.Create(_T("Tạo tài khoản"), WS_CHILD | WS_VISIBLE, rectSignUpButton, this, 1201);

	//sign_up_button.SetFaceColor(GetSysColor(COLOR_3DFACE), FALSE); // Nền trùng với nền dialog (xóa hiệu ứng)
	//sign_up_button.SetTextColor(RGB(0, 0, 0)); // Chữ màu đen
	CClientDC dc(this); 
	CRect rClient; 
	GetClientRect(&rClient); 
	UINT width = rClient.Width(); 
	UINT height = rClient.Height(); 


	m_fontLogo.CreateFont(
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
	m_fontText.CreateFont(
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


	m_fontTextInput.CreateFont(
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


	CRect rSL = CalcHelper::CalcTextRect(&dc, _T("BKAV Chat"), &m_fontLogo); // rect static text logo 
	m_textLogo.Create(_T("BKAV Chat"), WS_CHILD | WS_VISIBLE, rSL, this, 1201);
	m_textLogo.SetFont(&m_fontLogo, true);


	// Fullname

	CRect rSF = CalcHelper::CalcTextRect(&dc, _T("Tên hiển thị"), &m_fontText); // rect static text fullname 
	UINT tmpH = rSF.Height();
	UINT tmpW = rSF.Width();
	rSF.top = (height / 6);
	rSF.bottom = rSF.top + tmpH;
	rSF.left = width / 7;
	rSF.right = rSF.left + tmpW;
	m_textFullname.Create(_T("Tên hiển thị"), WS_CHILD | WS_VISIBLE, rSF, this, 1202);
	m_textFullname.SetFont(&m_fontText, true);

	CRect rTF( (width/3) + rSF.left, rSF.top, 6 * (width / 7), rSF.top + rSF.Height()); 
	m_editFullname.Create(WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER, rTF, this, 1203);
	m_editFullname.SetFont(&m_fontTextInput,  true);
	
	// Account

	CRect rSA = CalcHelper::CalcTextRect(&dc, _T("Tài khoản"), &m_fontText); // rect static text password
	tmpH = rSA.Height();
	tmpW = rSA.Width();
	rSA.top = 2 * (height / 5) - (height / 10);
	rSA.bottom = rSA.top + tmpH;
	rSA.left = rSF.left;
	rSA.right = rSA.left + tmpW;

	m_textAccount.Create(_T("Tài khoản"), WS_CHILD | WS_VISIBLE, rSA, this, 1204);
	m_textAccount.SetFont(&m_fontText, true);

	CRect rTA(rTF.left, rSA.top, rTF.right , rSA.top + rSA.Height());  // rect text edit password
	m_editAccount.Create(WS_CHILD | WS_VISIBLE  | ES_LEFT | WS_BORDER, rTA, this, 1205);
	m_editAccount.SetFont(&m_fontTextInput,  true);
	

	// Password
	CRect rSP = CalcHelper::CalcTextRect(&dc, _T("Mật khẩu"), &m_fontText); // rect static text password
	tmpH = rSP.Height();
	tmpW = rSP.Width();
	//rSP.top = 3 * (height / 5) - (height / 8);
	rSP.top =  rSA.top +  (( 8 * height )/60);
	rSP.bottom = rSP.top + tmpH;
	rSP.left = rSF.left;
	rSP.right = rSP.left + tmpW;

	m_textPassword.Create(_T("Mật khẩu"), WS_CHILD | WS_VISIBLE, rSP, this, 1206);
	m_textPassword.SetFont(&m_fontText, true);

	CRect rTP(rTF.left, rSP.top, rTF.right, rSP.top + tmpH);  // rect text edit password
	m_editPassword.Create(WS_CHILD | WS_VISIBLE | ES_PASSWORD | ES_LEFT | WS_BORDER, rTP, this, 1207 );
	m_editPassword.SetFont(&m_fontTextInput, true);

	// Re-password
	CRect rSR =  CalcHelper::CalcTextRect(&dc, _T("Nhập lại mật khẩu"), &m_fontText); // rect static text password
	tmpH = rSR.Height();
	tmpW = rSR.Width();

	rSR.top = 4 * (height / 5) - (height / 8);
	rSR.top = rSP.top + ((8 * height) / 60); 

	rSR.bottom = rSR.top + tmpH;
	rSR.left = rSF.left;
	rSR.right = rSR.left + tmpW;
	m_textRePassword.Create(_T("Nhập lại mật khẩu"), WS_CHILD | WS_VISIBLE, rSR, this, 1208);
	m_textRePassword.SetFont(&m_fontText, true);

	CRect rTR(rTF.left, rSR.top, rTF.right, rSR.top + rSR.Height());  // rect text edit password
	m_editRePassword.Create(WS_CHILD | WS_VISIBLE | ES_PASSWORD | ES_LEFT | WS_BORDER, rTR, this, 1209);
	m_editRePassword.SetFont(&m_fontTextInput, true);

	// Xong Button
	UINT widthButton = static_cast<UINT> (width / 4);
	UINT heightButton = static_cast<UINT> ((width / 14));

	CRect rSib;
	rSib.SetRectEmpty();
	rSib.left = (width - widthButton) / 2; // rect sign in button
	rSib.top = (height * 3) / 5  + (height / 7) ; // rect sign in button
	rSib.right = rSib.left + widthButton;  // rect sign in button
	rSib.bottom = rSib.top + heightButton; // rect sign in button

	sign_up_button.Create(_T("Tạo tài khoản"), rSib, this, 1210, &m_fontText, true);

	CRect noti_1 = CalcHelper::CalcTextRect(&dc, _T("Tài khoản đã tồn tại"), &m_fontText); 
	noti_1.MoveToXY((width - noti_1.Width()) / 2, rSib.bottom + (height / 15));
	accountExist.Create(_T("Tài khoản đã tồn tại !"), WS_CHILD | WS_VISIBLE, noti_1, this, 1211);
	accountExist.SetFont(&m_fontText); 
	accountExist.ShowWindow(SW_HIDE);

	CRect noti_2 = CalcHelper::CalcTextRect(&dc, _T("Đăng ký thành công, cửa sổ sẽ đóng sau 5s"), &m_fontText); 
	noti_2.MoveToXY((width - noti_2.Width()) / 2, rSib.bottom + (height / 15));
	registerSuccessfully.Create(_T("Đăng ký thành công, cửa sổ sẽ đóng sau 5s"), WS_CHILD | WS_VISIBLE, noti_2, this, 1212);
	registerSuccessfully.SetFont(&m_fontText); 
	registerSuccessfully.ShowWindow(SW_HIDE);

	CRect noti_3 = CalcHelper::CalcTextRect(&dc, _T("Mật khẩu không khớp !"), &m_fontText); 
	noti_3.MoveToXY((width - noti_3.Width()) / 2, rSib.bottom + (height / 15));
	passwordMissmatch.Create(_T("Mật khẩu không khớp !"), WS_CHILD | WS_VISIBLE, noti_3, this, 1213);
	passwordMissmatch.SetFont(&m_fontText); 
	passwordMissmatch.ShowWindow(SW_HIDE);

	
	return TRUE;
}


void CRegisterView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 0 :
		registerSuccessfully.ShowWindow(SW_HIDE); 
		KillTimer(0); 
		CDialog::OnOK();
		break;
	case 1 : 
		accountExist.ShowWindow(SW_HIDE); 
		KillTimer(1); 
		break;
	case 2 :
		passwordMissmatch.ShowWindow(SW_HIDE);
		KillTimer(2);
		break;
	}

}
LRESULT CRegisterView::OnResponse(WPARAM wParam, LPARAM lParam)
{
	
	CString* pResponse = (CString*)lParam;
	CT2A rev(*pResponse); 
	std::string str = std::string(rev);
	nlohmann::json j;
	j = nlohmann::json::parse(str);

	if (j["status"] == 0)
	{
		accountExist.ShowWindow(SW_SHOW); 
		SetTimer(1, 3000, nullptr);
	}
	else
	{
		registerSuccessfully.ShowWindow(SW_SHOW); 
		SetTimer(0, 5000, nullptr); 
	}

	delete pResponse; 


	return 0;

}



HBRUSH CRegisterView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	UINT nID = pWnd->GetDlgCtrlID();
	if (nID == 1201)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(28, 127, 217)); 
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	} else 
		if (nID == 1211 || nID == 1212 || nID == 1213)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(236, 70, 34));
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor); 
}

BEGIN_MESSAGE_MAP(CRegisterView, CDialogEx)
	ON_BN_CLICKED(1210, &CRegisterView::SignUpButtonClicked)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_API_REGISTER, &CRegisterView::OnResponse)
END_MESSAGE_MAP()


// CRegisterView message handlers

