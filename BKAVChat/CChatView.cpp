// CChatView.cpp : implementation file
//

#include "pch.h"
#include "BKAVChat.h"
#include "afxdialogex.h"
#include "CChatView.h"


// CChatView dialog
// 1400

IMPLEMENT_DYNAMIC(CChatView, CDialogEx)

CChatView::CChatView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATVIEW, pParent)
{

}

CChatView::~CChatView()
{
}

void CChatView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CChatView::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER);
	this->SetWindowTextW(_T("BKAV Chat - ") + user.showName);
	CRect rClient;
	GetClientRect(&rClient);
	UINT width = rClient.Width();
	UINT height = rClient.Height();
	m_font.CreateFont(
		height / 30,                    // Chiều cao font (pixel)
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

	m_fontDownload.CreateFont(
		height / 30,                    // Chiều cao font (pixel)
		0,                     // Chiều rộng (0 = tự tính)
		0, 0,                  // Escapement, Orientation
		FW_BOLD,               // Đậm
		FALSE,                 // Italic
		TRUE,                 // Underline
		0,                     // Strikeout
		ANSI_CHARSET,          // Charset (dùng ANSI nếu không cần Unicode đặc biệt)
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		_T("Segoe UI")         // Tên font
	);
	CRect rLM = rClient;
	rLM.bottom -= (height / 12);
	listMessage.Create(rLM, this, 1401, &m_font, &m_fontDownload);



	CRect rI(0, rLM.bottom, rClient.right - (4 * (height / 12)), rClient.bottom);

	input.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, rI, this, 1402);
	input.SetFont(&m_font);
	CRect rS(rI.right, rI.top, rI.right + (height / 12),rI.bottom);
	CRect rE(rS.right, rI.top, rS.right + (height / 12 ), rI.bottom);
	CRect rImg(rE.right, rI.top, rE.right + (height / 12) , rI.bottom);
	CRect rF(rImg.right, rI.top, rImg.right + (height / 12 ), rI.bottom);

	m_hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME); 
	m_sendButton.Create(&m_hIcon, rS, this, 1403);
	m_emojiButton.Create(&m_hIcon, rE, this, 1404);
	m_imageButton.Create(&m_hIcon, rImg, this, 1405);
	m_fileButton.Create(&m_hIcon, rF, this, 1406);


	APIHelper::GetMessageFromServer(this->GetSafeHwnd(), user.friendId);

	return TRUE;
}
void CChatView::OnSendButtonClicked()
{
	input.GetWindowText(m_inputController); 
	AfxMessageBox(m_inputController); 
	APIHelper::SendMessageTo(this->GetSafeHwnd(), m_inputController);

	input.SetWindowTextW(_T(""));
}
void CChatView::OnEmojiButtonClicked()
{
	if (m_pEmojiPicker == nullptr)
	{
		m_pEmojiPicker = new CEmojiPicker(this);
		m_pEmojiPicker->Create(IDD_EMOJIPICKER, this);

		// Hiển thị tại vị trí phù hợp
		CRect rcBtn;
		GetDlgItem(1404)->GetWindowRect(&rcBtn);
		m_pEmojiPicker->SetWindowPos(NULL, rcBtn.left, rcBtn.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		m_pEmojiPicker->ShowWindow(SW_SHOW);
		m_pEmojiPicker->SetFocus();
	}
}
void CChatView::EmojiHandle(UINT sel)
{
	input.GetWindowTextW(m_inputController); 

	CString emoji; 
	switch (sel)
	{
	case 1:
		emoji = _T(":)");
		break;
	case 2:
		emoji = _T(":>");
		break;
	case 3:
		emoji = _T(":D");
		break;
	case 4:
		emoji = _T(":'(");
		break;
	case 5:
		emoji = _T(">:(");
		break;
	case 6:
		emoji = _T(":P");
		break;
	case 7:
		emoji = _T("O_O");
		break;
	case 8:
		emoji = _T(":3");
		break;
	case 9:
		emoji = _T(":-|");
		break;
	case 10:
		emoji = _T("^_^");
		break;
	case 11:
		emoji = _T("T_T");
		break;
	case 12:
		emoji = _T("xD");
		break;
	case 13:
		emoji = _T("<3");
		break;
	case 14:
		emoji = _T(":o");
		break;
	case 15:
		emoji = _T(":v");
		break;
	case 16:
		emoji = _T("-_-");
		break;
	default:
		emoji = _T("");
		break;
	}
	m_inputController += emoji; 

	input.SetWindowTextW(m_inputController); 
}
void CChatView::OnImageButtonClicked()
{
	LPCTSTR lpszFilter = _T("Image Files (*.jpg;*.jpeg;*.png)|*.jpg;*.jpeg;*.png|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, lpszFilter, AfxGetMainWnd());
	if (dlg.DoModal() == IDOK)
	{

		CString pathname = dlg.GetPathName();
		GlobalParam::files.push_back(pathname);
	}
	
}
void CChatView::OnFileButtonClicked()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{

		CString pathname = dlg.GetPathName();
		GlobalParam::files.push_back(pathname);
	}
}
LRESULT CChatView::OnResponseSend(WPARAM wParam, LPARAM lParam)
{
	// Gui tin nhan -> nhan Response 
	// 1. Phan tich Response thanh List cac message 
	// 2. Them Message vao LOCAL 
	// 3. Them Message vao cache 
	// 4. Chi them cac Message moi nay vao ListMessage, vi ListMessage da khoi tao roi 
	std::string* pResponse = reinterpret_cast<std::string*>(lParam);
	nlohmann::json j; 
	j = nlohmann::json::parse(*pResponse); 
	nlohmann::json tmp = j["data"]; 
	std::string tmp_str = tmp.dump(); 
	std::vector<Entities::Message> tmpVector; 

	tmpVector = MessageHelper::Json2Message(tmp_str, this, user.friendId, &m_font); 
	MessageHelper::WaitResource(tmpVector); // Chac chan phai co tin nhan
	// Ghi vao trong LOCAL 
	DatabaseManager::GetInstance().InsertMessage(tmpVector); 

	// Luu vao trong cache
	SaveMessageIntoCache(tmpVector, false); 
	
	AddItemToListMessage(tmpVector); 
	delete pResponse; 
	return 0;
 
}
void CChatView::OnSelChange()
{

	int sel = listMessage.GetCurSel(); 
	if (sel != LB_ERR)
	{
		Entities::Message* message = listMessage.GetMessageAt(sel);
		if (message->type == 2 || message->type == 1)
		{
			//AfxMessageBox(message->text);
			CString ext = PathFindExtension(message->content);   // .txt
			ext.TrimLeft('.');
			CFileDialog dlg(FALSE, ext , message->content,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All file (*.*)|*.*||"), this);
			if (dlg.DoModal() == IDOK)
			{
				CString savePath = dlg.GetPathName();
				APIHelper::GetResource(this->GetSafeHwnd(), message->link, savePath);

			}
			
		} 
			
	}
}

void CChatView::ReceivedData(Entities::User userReceived )
{
	user = userReceived; 
}
LRESULT CChatView::OnResponseGetAll(WPARAM wParam, LPARAM lParam)
{
	// ep lai kieu con tro
	//CString* pFriendID = (CString*)pFriendID; // Su dung FriendID thi *pFriendID 

	std::string* pResponse = reinterpret_cast<std::string*> (lParam) ;
	std::vector<Entities::Message> allVector;
	//// convert to TCHAR* to ANSI 
	nlohmann::json j;
	j = nlohmann::json::parse(*pResponse);
	// Ket qua tra ve la 1 list cac json trong truong "Data"

	std::vector<Entities::Message> tmpVector;
	for (const auto& item : j["data"]) 
	{
		std::string param = item.dump();
		tmpVector = MessageHelper::Json2Message(param, this, user.friendId, &m_font); // tra ve 1 vector to, sau do them no vao vector chinh
		allVector.insert(allVector.end(), tmpVector.begin(), tmpVector.end()); 
	}
	
	MessageHelper::WaitResource(allVector);
	
	// Luu vao database 
	DatabaseManager::GetInstance().InsertMessage(allVector);

	SaveMessageIntoCache(allVector, true);

	delete pResponse;
	return 0;
}

LRESULT CChatView::OnResponseGetLast(WPARAM wParam, LPARAM lParam)
{	
	// Khong tin nhan moi: 
	// Kiem tra trong cache 
	// - Da co messages[friendId] thi bo qua
	// - Chua co 
	//		1. Lay toan bo tin nhan tu local 
	//		2. Luu vao trong cache 
	// Khong tin nhan moi: 
	// Kiem tra trong cache 
	// - Da co messages[friendId] thi bo qua
	// - Chua co 
	//		
	// -----------------------
	// Co tin nhan moi : 
	// Luu tin nhan nhan duoc vao LOCAL
	// Neu trong cache 
	//		- Da co messages[friendId] ( chua tin nhan ), thi chi can insert vao cuoi 
	//		- Chua co messages[friendId] 
	//			1. Lay toan bo tin nhan tu LOCAL 
	//			2. Luu vao trong cache

	std::string* pResponse = reinterpret_cast<std::string*> (lParam); 

	nlohmann::json j; 
	j = nlohmann::json::parse(*pResponse);
	std::vector<Entities::Message> allVector;

	std::vector<Entities::Message> tmpVector;
	for (const auto& item : j["data"])
	{
		std::string param = item.dump();
		tmpVector = MessageHelper::Json2Message(param, this, user.friendId, &m_font); 
		allVector.insert(allVector.end(), tmpVector.begin(), tmpVector.end()); 
	}

	// Doi tai ve cac tai nguyen ( hinh anh ) 
	MessageHelper::WaitResource(allVector); 
	//CString format; 
	//format.Format(_T("So tin nhan moi: %d"), allVector.size()); 
	//AfxMessageBox(format); 
	// Them tin nhan vao LOCAL 
	DatabaseManager::GetInstance().InsertMessage(allVector); 
	
	// Neu 
	// 		- Co tin nhan moi 
	//		- Khong co tin nhan moi

	SaveMessageIntoCache(allVector, true); 
	delete pResponse;
	return 0;
}

void CChatView::SaveMessageIntoCache(std::vector<Entities::Message> vt, bool isCreate)
{
	// Neu friendId 
	// - Da ton tai trong cache, them vao cuoi 
	// - Chua ton tai :	
	//		+ Neu queue > 5, thi xoa friendId cho vao som nhat
	//		+ Neu queue < 5, them vao binh thuong 
	std::vector<Entities::Message> vt_allMsg;
	if (GlobalParam::messages.find(user.friendId) == GlobalParam::messages.end() )
	{
		// Neu queueKey co kich thuoc > 5 thi xoa di 1 cai 
		if (GlobalParam::queueKey.size() > 5)
		{
			// Lay phan tu dau tien cua hang doi 
			CString key = GlobalParam::queueKey.front();

			// Xoa phan tu dau tien ( friendId cho vao som nhat )
			GlobalParam::queueKey.pop();

			// Xoa Messages[friendId]
			GlobalParam::messages.erase(key);
		}

		// Day friendId moi 
		GlobalParam::queueKey.push(user.friendId);
		vt_allMsg = DatabaseManager::GetInstance().GetMessageFromLocal(user.friendId);
		//CString format;
		//format.Format(_T("So tin nhan truy van ra : %d"), vt_allMsg.size());
		//AfxMessageBox(format);
	}
	else
		vt_allMsg = vt;

	// Problem : khi friendId da ton tai san thi no khong cap nhat lai
	//CString format;
	//format.Format(_T("So tin nhan them vao : %d"), vt_allMsg.size());
	//AfxMessageBox(format);
	GlobalParam::messages[user.friendId].insert(GlobalParam::messages[user.friendId].end(),  vt_allMsg.begin(), vt_allMsg.end());
	
	// neu da ton tai trong cach
	
	// cap nhat lai lastSynced ( bằng tin nhắn cuối ) 
	if (GlobalParam::messages[user.friendId].size() != 0)
	{
		Entities::Message message = GlobalParam::messages[user.friendId].back();
		DatabaseManager::GetInstance().UpdateLastSynced(user.friendId, message.time);
	}

	// Goi den ham khoi tao message
	if (isCreate)
		AddItemToListMessage(GlobalParam::messages[user.friendId]);
}
void CChatView::AddItemToListMessage(std::vector<Entities::Message> &vt)
{
	for (const Entities::Message& item : vt)
	{
		listMessage.AddItem(item);
	}
	int count = listMessage.GetCount(); 
	listMessage.SetTopIndex(count - 1);
}
void CChatView::OnOK()
{
	OnSendButtonClicked(); 
}
BEGIN_MESSAGE_MAP(CChatView, CDialogEx)
	ON_BN_CLICKED(1403, &CChatView::OnSendButtonClicked)
	ON_BN_CLICKED(1404, &CChatView::OnEmojiButtonClicked)
	ON_BN_CLICKED(1405, &CChatView::OnImageButtonClicked)
	ON_BN_CLICKED(1406, &CChatView::OnFileButtonClicked)
	ON_MESSAGE(WM_RESPONSE_ALL_MSG, &CChatView::OnResponseGetAll)
	ON_MESSAGE(WM_RESPONSE_LAST_MSG, &CChatView::OnResponseGetLast)
	ON_MESSAGE(WM_API_SEND, &CChatView::OnResponseSend)

	ON_LBN_SELCHANGE(1401, &CChatView::OnSelChange)
END_MESSAGE_MAP()


// CChatView message handlers
