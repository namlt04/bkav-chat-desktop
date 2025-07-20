#include "pch.h"
#include "APIHelper.h"
#include "CHomeView.h"
#include "WindowMessageCustome.h"
#include <atlpath.h>
#include "BKAVChat.h"
// Khoi tao
std::queue<APIRequest> APIHelper::ApiQueue;
std::mutex APIHelper::QueueMutex;
std::condition_variable APIHelper::QueueCV;



bool APIHelper::StopThread = false;


void APIHelper::SendMessageTo(HWND hTargetWnd, CString msg, std::vector<Entities::Attach>* paths)
{
	// parameter : message 
	// 0 la hinh anh, 1 la file

	APIRequest request;
	request.path = _T("/api/message/send-message");
	std::string body; 
	//CStringA body; 
	body = "--01112323232321382312938271372614432\r\n";
	body += "Content-Disposition: form-data; name=\"FriendID\"\r\n\r\n";
	//body += CT2A(GlobalParam::current, CP_UTF8) + "\r\n"; 
	{
		int len = ::WideCharToMultiByte(CP_UTF8, 0, GlobalParam::current, -1, nullptr, 0, nullptr, nullptr);
		if (len > 0) {
			std::string utf8(len - 1, 0);
			::WideCharToMultiByte(CP_UTF8, 0, GlobalParam::current, -1, &utf8[0], len, nullptr, nullptr);
			body += utf8 + "\r\n";
		}
	}
	request.body.insert(request.body.end(), body.begin(), body.end());
	body = "--01112323232321382312938271372614432\r\n";
	body += "Content-Disposition: form-data; name=\"Content\"\r\n\r\n";
	//body += CT2A(msg, CP_UTF8) + "\r\n";
	{
		int len = ::WideCharToMultiByte(CP_UTF8, 0, msg, -1, nullptr, 0, nullptr, nullptr);
		if (len > 0) {
			std::string utf8(len - 1, 0); // Bỏ null terminator
			::WideCharToMultiByte(CP_UTF8, 0, msg, -1, &utf8[0], len, nullptr, nullptr);
			body += utf8 + "\r\n";
		}
	}

	request.body.insert(request.body.end(), body.begin(), body.end());
	// Gui file
	for (Entities::Attach& link : (*paths))
	{
		CPath path(link.path); 
		CStringA contentType = "";
		CString ext = path.GetExtension();
		//AfxMessageBox(ext); 
		if ( ext == _T(".png"))
			contentType = "Content-Type: image/png\r\n\r\n";
		else if (ext == _T(".jpg") || ext == _T(".jpeg"))
			contentType = "Content-Type: image/jpeg\r\n\r\n";
		else 
			contentType = "Content-Type: application/octet-stream\r\n\r\n";
		CStringA fileName = CStringA(PathFindFileName(link.path));
		body = "--01112323232321382312938271372614432\r\n";
		body += "Content-Disposition: form-data; name=\"files\"; filename=\"" + fileName + "\"\r\n";
		body += contentType;
		// Doc tung bit nhi phan cua file tu link
		request.body.insert(request.body.end(), body.begin(), body.end());
		CFile file; 
		if (file.Open(link.path, CFile::modeRead | CFile::typeBinary ))
		{
			const UINT bufsize = 4096;
			BYTE  buffer[bufsize]; 
			UINT read = 0; 
			while ((read = file.Read(buffer, bufsize)) > 0)
			{
				request.body.insert(request.body.end(), buffer,buffer +  read);
			}
			CStringA fileEnd = "\r\n";

			request.body.insert(request.body.end(), (BYTE*)fileEnd.GetString(), (BYTE*)fileEnd.GetString() + fileEnd.GetLength());
			file.Close();
		}
	}
	body = "--01112323232321382312938271372614432--\r\n";
	request.body.insert(request.body.end(), body.begin(), body.end());


	
	// Tao header 
	CString header;
	header.Format(_T("Authorization: Bearer %s\r\n"), GlobalParam::token.GetString()); 
	header = header + _T("Content-Type: multipart/form-data;charset=UTF-8;boundary=01112323232321382312938271372614432"); // boundary random
	request.headers = header;
	
	request.hTargetHwnd = hTargetWnd;
	request.messageId = WM_API_SEND;	// Đưa vào trong queue
	//AfxMessageBox(_T("Da gui vao trong queue"));
	ApiQueue.push(request); 
	QueueCV.notify_one();

}

void APIHelper::Login(HWND hTargetWnd, const CString& username, const CString& password)
{
	APIRequest request;
	nlohmann::ordered_json j; 
	j["Username"] = CT2A(username);
	j["Password"] = CT2A(password); 
	
	// Chuyen ve CStringA ( utf8 )
	std::string json_string = j.dump(); 
	CStringA utf8Body(json_string.c_str());
	// tu CStringA chuyen sang vector<BYTE>
	request.body.insert(request.body.end(), (BYTE*)utf8Body.GetString(), (BYTE*)utf8Body.GetString() + utf8Body.GetLength());
	CString path; 
	path = request.path +  _T("/auth/login");
	request.path = path;

	request.headers = _T("Content-Type: application/json\r\n");
	request.hTargetHwnd = hTargetWnd;
	request.messageId = WM_API_LOGIN; 
	ApiQueue.push(request);
	QueueCV.notify_one();


}
void APIHelper::Register(HWND hTargetWnd, const CString& email, const CString& username, const CString& password )
{
	APIRequest request;
	nlohmann::json j;
	j["Username"] = CT2A(username);
	j["FullName"] = CT2A(email);
	j["Password"] = CT2A(password);

	std::string json_string = j.dump();
	CStringA bodyUtf8(json_string.c_str());

	request.body.insert(request.body.end(), (BYTE*)bodyUtf8.GetString(), (BYTE*)bodyUtf8.GetString() + bodyUtf8.GetLength());
	CString path = request.path + _T("/auth/register");
	request.path = path;
	

	request.headers = _T("Content-Type: application/json\r\n");
	request.hTargetHwnd = hTargetWnd;
	request.messageId = WM_API_REGISTER;

	ApiQueue.push(request);
	QueueCV.notify_one();
	//AfxMessageBox(_T("Cho tai xuong xong, dam bao khong loi")); 



}
void APIHelper::GetMessageFromServer(HWND hTargetWnd, CString friendId)
{
	CString ret = DatabaseManager::GetInstance().CheckLastSynced(friendId); 
	if (!ret.IsEmpty())
	{
		APIHelper::GetLastMessage(hTargetWnd, friendId, ret); 
		return; 
	}
	APIRequest request;
	
	request.method = _T("GET");
	CString header; 
	header.Format(_T("Authorization: Bearer %s \r\n"), GlobalParam::token.GetString());
	request.headers = header;

	CString path; 
	path.Format(_T("/api/message/get-message?FriendID=%s"), friendId.GetString()); 
	request.path = path; 
	//AfxMessageBox(request.path); 

	request.hTargetHwnd = hTargetWnd;
	request.messageId = WM_RESPONSE_ALL_MSG;
	GlobalParam::current = friendId;
	
	ApiQueue.push(request);
	QueueCV.notify_one();

}
void APIHelper::GetLastMessage(HWND hTargetWnd, CString FriendID, CString LastTime)
{
	APIRequest request;

	
	request.method = _T("GET");
	CString header;
	header.Format(_T("Authorization: Bearer %s \r\n"), GlobalParam::token.GetString());
	request.headers = header;
	//AfxMessageBox(_T("Cho tai xuong xong, dam bao khong loi")); 


	CString path;
	path.Format(_T("/api/message/get-message?FriendID=%s&LastTime=%s"), FriendID.GetString(), LastTime.GetString());
	request.path = path;

	request.hTargetHwnd = hTargetWnd;
	request.messageId = WM_RESPONSE_LAST_MSG;
	GlobalParam::current = FriendID;

	ApiQueue.push(request);
	QueueCV.notify_one();


}

void APIHelper::UpdateInformation(HWND hTargetWnd,const CString& fullname, const CString& avatar )
{
	// POST 
	APIRequest request;
	
	request.path = _T("/api/user/update");
	nlohmann::json j;
	j["FullName"] = CT2A(fullname);
	j["Avatar"] = CT2A(avatar);


	std::string json_string = j.dump();
	CString body(CA2CT(json_string.c_str()));
	//request.body = body;

	CString header; 
	header.Format(_T("Authorization: Bearer %s \r\n"), GlobalParam::token.GetString()); 
	request.headers = header;

	request.hTargetHwnd = hTargetWnd;
	
	//request.messageId 

	ApiQueue.push(request);
	QueueCV.notify_one();

}
void APIHelper::GetFriend(HWND hTargetWnd)
{
	APIRequest request; 
	request.method = _T("GET");
	request.path = _T("/api/message/list-friend"); 
	
	CString header; 
	header.Format(_T("Authorization : Bearer %s \r\n"), GlobalParam::token.GetString()); // yeu cau lpctstr
	request.headers = header; 
	request.hTargetHwnd = hTargetWnd;
	request.messageId = WM_API_FRIEND;
	ApiQueue.push(request); 
	QueueCV.notify_one();
}
void APIHelper::GetInformation(HWND hTargetWnd)
{
	APIRequest request; 
	request.method = _T("GET");
	request.path = _T("api/user/info");
	request.headers = _T("");
	ApiQueue.push(request); 
	QueueCV.notify_one();
}


void APIHelper::GetResource(HWND hTargetHwnd,CString url, CString saveUrl)
{
	APIRequest request; 
	request.method = _T("GET_RESOURCE");

	CString path = request.path;
	request.path = _T("http://") + request.host + _T(":8888") + path + url;
	request.headers = saveUrl;
	request.hTargetHwnd = hTargetHwnd;
	ApiQueue.push(request); 
	QueueCV.notify_one();
}

void APIHelper::AutoGetResource(HWND hTargetHwnd, CString url)
{
	APIRequest request;
	request.method = _T("AUTO_GET_RESOURCE");
	CString path = request.path;
	request.path = _T("http://") + request.host + _T(":8888") + path + url;
	//AfxMessageBox(request.path); 
	url.Replace('/', '\\'); 
	request.headers = FileManager::m_path + url;
	request.hTargetHwnd = hTargetHwnd;  
	ApiQueue.push(request);
	QueueCV.notify_one();
}

UINT APIHelper::NetworkThreadHandle(LPVOID pParam)
{
	while (!stopThread)
	{
		APIRequest request;
		bool hasRequest = false;
		{
			std::unique_lock<std::mutex> lock(QueueMutex);

			if (ApiQueue.empty())
			{
				//QueueCV.wait_for(lock, std::chrono::seconds(1));
				QueueCV.wait_for(lock, std::chrono::seconds(1), [] { return stopThread || !ApiQueue.empty(); });
			}
			if (stopThread) break;
			if (!ApiQueue.empty())
			{
				request = ApiQueue.front();
				ApiQueue.pop();
				hasRequest = true;
			}

			//CheckNetwork();  Ham xu li mang, dat trong 1 class khac


		}
		if (hasRequest)
		{
			SendApi(request);
		}
	}
	return 0;
}
void APIHelper::SendApi(APIRequest& request)
{
	if (request.method == _T("AUTO_GET_RESOURCE"))
	{
		//std::lock_guard<std::mutex> lock(downloadingMutex);
		//downloading.push(true);
		//	URLDownloadToFile(NULL, request.path, request.headers, 0, NULL);
		//	std::lock_guard<std::mutex> lock(downloadingMutex);
		//	if (!downloading.empty()) {
		//		downloading.pop();
		//	}

			//{
			//	// Lock mutex, push true khi bắt đầu tải
			//	std::lock_guard<std::mutex> lock(downloadingMutex);
			//	downloading.push(true);
			//}

			// Tải file bên ngoài mutex (không giữ mutex lâu)
			URLDownloadToFile(NULL, request.path, request.headers, 0, NULL);

			//{
			//	// Lock mutex, pop khi tải xong
			//	std::lock_guard<std::mutex> lock(downloadingMutex);
			//	if (!downloading.empty()) {
			//		downloading.pop();
			//	}
			//}
			// Nếu có condition_variable thì notify UI hoặc thread khác ở đây
	}
	else  
		if (request.method == _T("GET_RESOURCE"))
		{
			URLDownloadToFile(NULL, request.path, request.headers, 0, NULL);
			CString noti; 
			noti.Format(_T("Tai xuong thanh cong tai %s"), request.headers); 
			AfxMessageBox(noti); 
		} else
	{
		std::string response;
		CInternetSession session(_T("BKAVChat"));
		CHttpConnection* pConnection = nullptr;
		CHttpFile* pFile = nullptr;

		//CStringA utf8Data;
		std::string utf8Data;
		char buffer[4096];
		UINT bytesRead = 0;

		try
		{
			UINT method = CHttpConnection::HTTP_VERB_POST;
			if (request.method == _T("GET"))
				method = CHttpConnection::HTTP_VERB_GET;
			pConnection = session.GetHttpConnection(request.host, INTERNET_FLAG_DONT_CACHE, request.port);
			pFile = pConnection->OpenRequest(method, request.path, NULL, 0, NULL, NULL, INTERNET_FLAG_RELOAD);

			//pFile->SendRequest(request.headers, (LPVOID)(LPCSTR)(request.body), (request.body).GetLength());
			// Chuyen sang su dung vector<BYTE>
			pFile->SendRequest(request.headers, request.body.data(), request.body.size()); 
			


			char buffer[4096];
			UINT bytesRead = 0;
			while ((bytesRead = pFile->Read(buffer, sizeof(buffer) - 1)) > 0)
			{
				buffer[bytesRead] = '\0';
				utf8Data += buffer;
			}
			response = utf8Data;
		}
		catch (CInternetException* e) // Bat Exception bang con tro
		{

		}
		if (pFile) delete pFile;
		if (pConnection) delete pConnection;
		session.Close();
		//AfxMessageBox(response);
		//CString* pResponse = new CString(response);
		std::string* pResponse = new std::string(response); 
		//AfxMessageBox(_T("Nhan thong tin ")); 
		::PostMessage(request.hTargetHwnd, request.messageId, 0, reinterpret_cast<LPARAM>(pResponse));
	}
}
