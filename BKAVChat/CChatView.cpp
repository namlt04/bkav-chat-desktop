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
	CRect rect;
	CRect clientRect;
	GetClientRect(&clientRect);

	GetClientRect(&rect);
	rect.bottom -= 50;
	listMessage.Create(rect, this, 1401);

	/*for (const Message& item : GlobalParam::messages[GlobalParam::current])
	{
		listMessage.AddItem(item);
	}*/


	CRect inputRect(0, rect.bottom,clientRect.right - 200 , clientRect.bottom);
	
	input.Create(WS_CHILD | WS_VISIBLE, inputRect, this, 1402);
	CRect sendRect(clientRect.right - 200, rect.bottom, clientRect.right - 150, clientRect.bottom); 
	CRect emoijRect(clientRect.right - 150, rect.bottom, clientRect.right - 100, clientRect.bottom); 
	CRect imageRect(clientRect.right - 100, rect.bottom, clientRect.right - 50, clientRect.bottom);
	CRect fileRect(clientRect.right - 50, rect.bottom, clientRect.right, clientRect.bottom);

	sendButton.Create(_T("Send"), WS_CHILD | WS_VISIBLE, sendRect, this, 1403); 
	emoijButton.Create(_T("Emoiji"), WS_CHILD | WS_VISIBLE, emoijRect, this, 1404);
	imageButton.Create(_T("Img"), WS_CHILD | WS_VISIBLE, imageRect, this, 1405);
	fileButton.Create(_T("File"), WS_CHILD | WS_VISIBLE, fileRect, this, 1406);


	APIHelper::GetMessage(this->GetSafeHwnd(), user.friendId);

	return TRUE;
}
void CChatView::OnSendButtonClicked()
{
	input.GetWindowText(m_inputController); 
	APIHelper::SendMessageTo(this->GetSafeHwnd(), m_inputController);
	input.SetWindowTextW(_T(""));
}
void CChatView::OnEmoijButtonClicked()
{
	// hien thi bang chon
}
void CChatView::OnImageButtonClicked()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("*.jpg; *.jpeg;*.png"), AfxGetMainWnd());
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
	CString* pResponse = (CString*)lParam;
	std::string str_response = std::string(CT2A(*pResponse));
	nlohmann::json j; 
	j = nlohmann::json::parse(str_response); 
	nlohmann::json tmp = j["data"]; 
	std::string tmp_str = tmp.dump(); 
	MessageHelper::Json2Message(tmp_str, this); 
	AfxMessageBox(_T("Da them tin nhan moi"));
	//MessageHelper::WaitResource();
	//Message message = GlobalParam::messages[GlobalParam::current].back();

	//listMessage.AddItem(message);
	delete pResponse; 
	return 0;
 
}
void CChatView::OnSelChange()
{

	int sel = listMessage.GetCurSel(); 
	if (sel != LB_ERR)
	{
		Message* message = listMessage.GetMessageAt(sel);
		if (message->type == 2)
		{
			AfxMessageBox(message->text);
			CString ext = PathFindExtension(message->text);   // .txt
			ext.TrimLeft('.');
			CFileDialog dlg(FALSE, ext , message->text,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"), this);
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
LRESULT CChatView::OnResponseGetAllMessages(WPARAM wParam, LPARAM lParam)
{
	// ep lai kieu con tro
	//CString* pFriendID = (CString*)pFriendID; // Su dung FriendID thi *pFriendID 

	CString* pResponse = (CString*)lParam;
	std::vector<Entities::Message> allVector;
	//// convert to TCHAR* to ANSI 
	CString tmp = *pResponse;
	CT2A converter(tmp);
	std::string str(converter);
	nlohmann::json j;
	j = nlohmann::json::parse(str);
	// Ket qua tra ve la 1 list cac json trong truong "Data"

	std::vector<Entities::Message> tmpVector;
	for (const auto& item : j["data"]) 
	{
		std::string param = item.dump();
		tmpVector = MessageHelper::Json2Message(param, this); // tra ve 1 vector to, sau do them no vao vector chinh
		allVector.insert(allVector.end(), tmpVector.begin(), tmpVector.end()); 
	}
	
	MessageHelper::WaitResource(allVector);
	
	CString form = _T("So luong message %d"); 
	form.Format(form, allVector.size()); 
	AfxMessageBox(form); 
	// Luu vao database 
	DatabaseManager::GetInstance().InsertMessage(allVector);

	SaveMessageIntoCache(allVector);


	return 0;
}

LRESULT CChatView::OnResponseGetLastMessages(WPARAM wParam, LPARAM lParam)
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

	CString* pResponse = (CString*)lParam;

	CT2A converter(*pResponse);
	std::string str(converter);
	nlohmann::json j; 
	j = nlohmann::json::parse(str);
	std::vector<Entities::Message> allVector;

	std::vector<Entities::Message> tmpVector;
	for (const auto& item : j["data"])
	{
		std::string param = item.dump();
		tmpVector = MessageHelper::Json2Message(param, this); 
		allVector.insert(allVector.end(), tmpVector.begin(), tmpVector.end()); 
	}

	// Doi tai ve cac tai nguyen ( hinh anh ) 
	MessageHelper::WaitResource(allVector); 

	// Them tin nhan vao LOCAL 
	DatabaseManager::GetInstance().InsertMessage(allVector); 
	
	// Neu 
	// 		- Co tin nhan moi 
	//		- Khong co tin nhan moi

	SaveMessageIntoCache(allVector); 
	return 0;
}

void CChatView::SaveMessageIntoCache(std::vector<Entities::Message> vt)
{
	// Neu friendId 
	// - Da ton tai trong cache, them vao cuoi 
	// - Chua ton tai :	
	//		+ Neu queue > 5, thi xoa friendId cho vao som nhat
	//		+ Neu queue < 5, them vao binh thuong 
	std::vector<Entities::Message> vt_allMsg;
	if (GlobalParam::messages.find(user.friendId) != GlobalParam::messages.end() )
	{
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
		vt_allMsg = DatabaseManager::GetInstance().GetMessage(user.friendId);
	}
	else
		vt_allMsg = vt;
	// Problem : khi friendId da ton tai san thi no khong cap nhat lai

	GlobalParam::messages[user.friendId].insert(GlobalParam::messages[user.friendId].end(),  vt_allMsg.begin(), vt_allMsg.end());
	
	// neu da ton tai trong cach
	
	// cap nhat lai lastSynced ( bằng tin nhắn cuối ) 
	Entities::Message message = GlobalParam::messages[user.friendId].back(); 
	DatabaseManager::GetInstance().UpdateLastSynced(user.friendId, message.time); 

	// Goi den ham khoi tao message

	for (const Entities::Message& item : GlobalParam::messages[user.friendId])
	{
		listMessage.AddItem(item);
	}
}

BEGIN_MESSAGE_MAP(CChatView, CDialogEx)
	ON_BN_CLICKED(1403, &CChatView::OnSendButtonClicked)
	ON_BN_CLICKED(1404, &CChatView::OnEmoijButtonClicked)
	ON_BN_CLICKED(1405, &CChatView::OnImageButtonClicked)
	ON_BN_CLICKED(1406, &CChatView::OnFileButtonClicked)
	ON_MESSAGE(WM_RESPONSE_ALL_MSG, &CChatView::OnResponseGetAllMessages)
	ON_MESSAGE(WM_RESPONSE_LAST_MSG, &CChatView::OnResponseGetLastMessages)
	ON_MESSAGE(WM_API_SEND, &CChatView::OnResponseSend)
	ON_LBN_SELCHANGE(1401, &CChatView::OnSelChange)
END_MESSAGE_MAP()


// CChatView message handlers
