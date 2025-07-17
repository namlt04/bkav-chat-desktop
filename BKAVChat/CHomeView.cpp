// CHomeView.cpp : implementation file
#include "pch.h"
#include "BKAVChat.h"
#include "afxdialogex.h"
#include "CHomeView.h"


// CHomeView dialog

IMPLEMENT_DYNAMIC(CHomeView, CDialogEx)

CHomeView::CHomeView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOMEVIEW, pParent)
{

}

CHomeView::~CHomeView()
{
}

void CHomeView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CHomeView::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER);
	this->SetWindowTextW(_T("BKAV Chat - Trang chủ")); 

	//for (Friend& f : GlobalParam::friends)
	//{
		//CString str = f.username.GetString();
		//listFriend.AddItem(/*str, hIcon*/);
	//}
	
	//AfxMessageBox(_T("OnInitDialgo đã được gọi ")); 
	//InitializeData(); 
	// Chua su dung toi database
	//DatabaseManager::InitDatabase();
	//sqlite3* pDb = DatabaseManager::GetDB();
	InitUI(); 
	//InitializeData(); 
	PostMessage(WM_INITDATA); 
	return TRUE;
}
void CHomeView::InitUI()
{
	CRect rect;
	GetClientRect(&rect);
	UINT width = rect.Width();
	UINT height = rect.Height();

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
	CClientDC dc(this);


	CRect rSL = CalcHelper::CalcTextRect(&dc, _T("BKAV Chat"), &m_fontLogo); // rect static text logo 
	m_logo.Create(_T("BKAV Chat"), WS_CHILD | WS_VISIBLE, rSL, this, 1301);
	m_logo.SetFont(&m_fontLogo, true);

	CRect rSB(width / 10, rSL.bottom + height / 40, 9 * (width / 10) - (height / 15) - (width / 60), rSL.bottom + height / 40 + height / 15); // rect Search Bar
	m_searchBar.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, rSB, this, 1302);
	m_searchBar.SetFont(&m_fontTextInput);

	m_hiSearch = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
	CRect rSBu(9 * (width / 10) - (height / 15), rSB.top, 9 * (width / 10), rSB.bottom); // rect Search Button
	m_cbSearch.Create(&m_hiSearch, rSBu, this, 1303);


	// 
	CRect rectInfoBar = CalcHelper::CalcTextRect(&dc, _T("Danh sách bạn bè"), &m_fontText);
	rectInfoBar.MoveToXY(rSB.left, rSB.bottom + (height / 60));

	m_infoBar.Create(_T("Danh sách bạn bè"), WS_CHILD | WS_VISIBLE, rectInfoBar, this, 1304);
	m_infoBar.SetFont(&m_fontText);

	CRect rectListBar(width / 10, rectInfoBar.bottom + (height / 60), 9 * (width / 10), 19 * (height / 20));
	listFriend.Create(rectListBar, this, height, 1305);
	//listFriend.SetFont(&m_fontText);
	HICON hIcon = AfxGetApp()->LoadIconW(IDI_AVATAR);
}
void CHomeView::OnSelChangeListFriend()
{
	int sel = listFriend.GetCurSel(); 
	//if (sel != LB_ERR)
	//{
	//	CString friendId = listFriend.GetUser(sel).friendId; 

	//	if (GlobalParam::messages.find(friendId) != GlobalParam::messages.end())
	//	{
	//		CString time = GlobalParam::messages[friendId].back().time;
	//		APIHelper::GetLastMessage(this->GetSafeHwnd(), friendId, time);
	//	}
	//	else
	//	{
	//		APIHelper::GetMessageAll(this->GetSafeHwnd(), friendId);
	//	}
	//	CString str;
	//	listFriend.GetText(sel, str); // Lấy nội dung dòng được chọn

	//	
	//}


	if (sel != LB_ERR)
	{
		Entities::User userSend = listFriend.GetUser(sel); 
		CChatView dlg; 
		dlg.ReceivedData(userSend); 
		dlg.DoModal(); 
	}
}

HBRUSH CHomeView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	UINT nId = pWnd->GetDlgCtrlID(); 
	if (nId == 1301)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(28, 127, 217));
		return (HBRUSH)GetStockObject(NULL_BRUSH); 
	}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor); 
}

LRESULT CHomeView::LateInitData(WPARAM wParam, LPARAM lParam)
{
	APIHelper::GetFriend(this->GetSafeHwnd()); 
	return 0;
}

LRESULT CHomeView::OnResponse(WPARAM wParam, LPARAM lParam)
{
	std::string* pResponse = reinterpret_cast<std::string*>(lParam); 

	nlohmann::json j; 
	//j = nlohmann::json::parse(str_response);

	try {
		j = nlohmann::json::parse(*pResponse);
	}
	catch (const std::exception& e) {
		AfxMessageBox(CA2T(e.what()));
		return -1;
	}

	for (const auto& item : j["data"])
	{
		Entities::User user;
		std::string tmp = item["FriendID"];
		user.friendId = CA2T(tmp.c_str(), CP_UTF8);

		tmp = item["Content"];
		CString content = CA2T(tmp.c_str(), CP_UTF8);
		if (content.IsEmpty())
		{
			if (item["Files"].is_array() && !item["Files"].empty())
			{
				CString temp;
				temp.Format(_T("%d tập tin"), (int)item["Files"].size());
				content += temp;
			}
			if (item["Images"].is_array() && !item["Images"].empty())
			{
				CString temp; 
				if (content.IsEmpty())
					temp.Format(_T("%d hình ảnh"), item["Images"].size());
				else
					temp.Format(_T(" và %d hình ảnh"), item["Images"].size());
				content += temp; 
			}
		}

		if (content.IsEmpty()) 
			content = _T("Chưa có tin nhắn.");
		else content += _T("."); 
		user.content = content; 

		user.isOnline = item["isOnline"];

		if (item.contains("FullName"))
		{
			tmp = item["FullName"]; 
			user.fullname = CA2T(tmp.c_str(), CP_UTF8);
		}
		if (item.contains("Avatar"))
		{
			tmp = item["Avatar"];
			user.avatar = CA2T(tmp.c_str(), CP_UTF8);
		}

		tmp = item["Username"];
		user.username = CA2T(tmp.c_str(), CP_UTF8);

		listFriend.AddItem(user); 

	}
	delete pResponse; 
	return 0;
}
void CHomeView::OnClose()
{
	CDialogEx::OnClose(); 
	AfxGetApp()->m_pMainWnd = nullptr;
}
BEGIN_MESSAGE_MAP(CHomeView, CDialogEx)
	ON_LBN_SELCHANGE(1305, &CHomeView::OnSelChangeListFriend)
	ON_MESSAGE(WM_API_FRIEND, &CHomeView::OnResponse)
	ON_MESSAGE(WM_INITDATA, &CHomeView::LateInitData)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// nID bắt đầu từ 1300
// WM_custom bat dau tu 130


// CHomeView message handlers
