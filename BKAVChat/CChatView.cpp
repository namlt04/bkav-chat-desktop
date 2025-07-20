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
	m_hIcon = AfxGetApp()->LoadIconW(IDI_BKAV);
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
	SetIcon(m_hIcon, TRUE); 
	SetIcon(m_hIcon, FALSE); 
	CRect rClient;
	GetClientRect(&rClient);
	UINT width = rClient.Width();
	UINT height = rClient.Height();
	m_font.CreateFont(
		height / 20,                    // Chiều cao font (pixel)
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
		height / 20,                    // Chiều cao font (pixel)
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



	rInput = CRect(width/12, rLM.bottom, rClient.right - (3 * (height / 12)) - (width / 12) ,rClient.bottom);
	CRect rI( width / 12, rLM.bottom, rClient.right - (4 * (height / 12)) - ( width / 12), rClient.bottom );

	input.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, rI, this, 1402);
	input.SetFont(&m_font);
	CRect rS(rI.right, rInput.top, rI.right + (height / 12),rInput.bottom);
	CRect rE(rS.right, rInput.top, rS.right + (height / 12 ), rInput.bottom);
	CRect rImg(rE.right, rInput.top, rE.right + (height / 12) , rInput.bottom);
	CRect rF(rImg.right, rInput.top, rImg.right + (height / 12 ), rInput.bottom);

	m_hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME); 
	m_hSend = AfxGetApp()->LoadIconW(IDI_SEND); 
	m_hAttach = AfxGetApp()->LoadIconW(IDI_ATTACH); 
	m_hPhoto = AfxGetApp()->LoadIconW(IDI_PHOTO); 
	m_hEmoji = AfxGetApp()->LoadIconW(IDI_EMOJI); 

	m_sendButton.Create(_T(""),  & m_hSend, rS, this, 1403, 1);
	m_emojiButton.Create(_T("") , & m_hEmoji, rE, this, 1404, 1);
	m_imageButton.Create(_T(""),  & m_hPhoto, rImg, this, 1405, 1);
	m_fileButton.Create(_T(""), & m_hAttach, rF, this, 1406, 1);


	//m_list.Create(CRect(10, 10, 500, 150), this, 1234);
	//m_list.ModifyStyle(0, WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_HSCROLL | WS_BORDER);
	m_list.Create(CRect(0, rLM.bottom - 150, width, rLM.bottom ), this, 1234);

	APIHelper::GetMessageFromServer(this->GetSafeHwnd(), user.friendId);

	//emojiPicker.Create(CRect(0, 0, 400, 400), this); 
	//emojiPicker.ShowWindow(SW_SHOW); 
	return TRUE;
}
void CChatView::OnSendButtonClicked()
{
	input.GetWindowText(m_inputController); 
	APIHelper::SendMessageTo(this->GetSafeHwnd(), m_inputController, m_list.GetFiles());
	input.SetWindowTextW(_T(""));

	if (m_list.IsWindowVisible())
		CloseFilePicker(0, 0);
}
LRESULT CChatView::CloseFilePicker(WPARAM wParam, LPARAM lParam)
{	
	CRect rML; 
	m_list.GetWindowRect(&rML);
	CRect rL; 
	listMessage.GetWindowRect(&rL);
	listMessage.MoveWindow(CRect(0, 0, rML.Width(), rML.Height() +  rL.Height()));
	listMessage.UpdateScrollBar(true);
	m_list.Close(); 
	return 0;
	
}
void CChatView::OnEmojiButtonClicked()
{
	if (pEmojiPicker == nullptr)
	{
		pEmojiPicker = new CEmojiPicker(this);
		pEmojiPicker->Create(IDD_EMOJIPICKER, this);

		// Hiển thị tại vị trí phù hợp
		CRect rcBtn;
		GetDlgItem(1404)->GetWindowRect(&rcBtn);
		pEmojiPicker->SetWindowPos(NULL, rcBtn.left, rcBtn.bottom, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		pEmojiPicker->ShowWindow(SW_SHOW);
		pEmojiPicker->SetFocus();
	}
}
void CChatView::EmojiHandle(CString emoji)
{
	input.GetWindowTextW(m_inputController); 
	m_inputController += emoji; 
	input.SetWindowTextW(m_inputController); 
}
void CChatView::OnImageButtonClicked()
{
	CRect r; 
	GetClientRect(&r); 
	CRect rListMessage; 
	listMessage.GetClientRect(&rListMessage); 
	if (!m_list.IsWindowVisible() )
		listMessage.MoveWindow(CRect(0,0,r.right, rListMessage.Height() - 150));
	m_list.ShowWindow(SW_SHOW);
	listMessage.UpdateScrollBar(true);
	m_list.BringWindowToTop(); 
	m_list.AddAttach(true); 
	
}
void CChatView::OnFileButtonClicked()
{
	CRect r; 
	GetClientRect(&r); 
	CRect rListMessage; 
	listMessage.GetClientRect(&rListMessage); 
	if (!m_list.IsWindowVisible() )
		listMessage.MoveWindow(CRect(0,0,r.right, rListMessage.Height() - 150));

	m_list.ShowWindow(SW_SHOW); 
	listMessage.UpdateScrollBar(true);
	m_list.BringWindowToTop(); 
	m_list.AddAttach(false); 
	
}
LRESULT CChatView::OnResponseSend(WPARAM wParam, LPARAM lParam)
{
	CRect tRect; 
	listMessage.GetClientRect(tRect); 
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
LRESULT CChatView::OnClickDownloadFile(WPARAM wParam, LPARAM lParam)
{

	//int sel = listMessage.GetCurSel(); 
	int sel = (int)wParam; 
	if (sel != LB_ERR)
	{
		Entities::Message message = listMessage.GetMessageAtSel(sel);
		if (message.type == 2 || message.type == 1)
		{
			//AfxMessageBox(message->text);
			CString ext = PathFindExtension(message.content);   // .txt
			ext.TrimLeft('.');
			CFileDialog dlg(FALSE, ext , message.content,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All file (*.*)|*.*||"), this);
			if (dlg.DoModal() == IDOK)
			{
				CString savePath = dlg.GetPathName();
				APIHelper::GetResource(this->GetSafeHwnd(), message.link, savePath);

			}
			
		} 
			
	}
	return 0; 
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
		listMessage.PushBackItem(item);
	}
	listMessage.SetScrollDown(); 
}
void CChatView::OnOK()
{
	OnSendButtonClicked(); 
}
BOOL CChatView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CChatView, CDialogEx)
	ON_BN_CLICKED(1403, &CChatView::OnSendButtonClicked)
	ON_BN_CLICKED(1404, &CChatView::OnEmojiButtonClicked)
	ON_BN_CLICKED(1405, &CChatView::OnImageButtonClicked)
	ON_BN_CLICKED(1406, &CChatView::OnFileButtonClicked)
	ON_MESSAGE(WM_RESPONSE_ALL_MSG, &CChatView::OnResponseGetAll)
	ON_MESSAGE(WM_RESPONSE_LAST_MSG, &CChatView::OnResponseGetLast)
	ON_MESSAGE(WM_API_SEND, &CChatView::OnResponseSend)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_USER + 800, &CChatView::OnClickDownloadFile)
	ON_MESSAGE(WM_USER + 500, &CChatView::CloseFilePicker) 

END_MESSAGE_MAP()


// CChatView message handlers
