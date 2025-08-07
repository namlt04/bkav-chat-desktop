#pragma once
#include "queue"
#include "afxinet.h"
#include "functional"
#include "mutex"
#include "condition_variable"
#include "json.hpp"
#include "GlobalParam.h"
#include "FileManager.h"
#include "DatabaseManager.h"
#include <vector>
struct APIRequest {
	CString method;
	CString host;
	CString path;
	INTERNET_PORT port;
	CString headers;
	std::vector<BYTE> body;
	HWND hTargetHwnd;
	UINT messageId;

	APIRequest(
		const CString& m = _T("POST"),
		const CString& h = _T("30.30.30.85"),
		const CString& p = _T("/api"),
		INTERNET_PORT pt = 8888,
		const CString& hdr = _T(""),
		std::vector<BYTE> body = {},
		HWND hWnd = nullptr, 
		UINT msgId = 0

	)
		: method(m), host(h), path(p), port(pt), headers(hdr), body(body), hTargetHwnd(hWnd), messageId(msgId){}
	
};
class APIHelper
{
	// Khoi tao tai ham .cpp vi no chi duoc khoi tao 1 lan duy nhat cho toan chuong trinh 

public:
	static std::queue<APIRequest> apiRequest;
	static std::mutex queueMutext; // tạo các các api request 

	static std::condition_variable queue;

	static void GetMessageFromServer(HWND hTargetWnd, CString FriendID); // tao cac api
	static void GetLastMessage(HWND hTargetWnd, CString FriendID, CString LastTime); // tao cac api

	static void SendMessageTo(HWND hTargetWnd, CString msg, std::vector<Entities::Attach>* paths);
	static void GetFriend(HWND hTargetWnd);
	static void GetResource(HWND hTargetWnd, CString url, CString saveUrl); 
	static void UpdateInformation(HWND hTargetWnd, const CString& fullname, const CString& avatar); 
	static void GetInformation(HWND hTargetWnd);
	static void Login(HWND hTargetWnd, const CString& username, const CString& password); 
	static void Register(HWND hTargetWnd, const CString& email, const CString& username, const CString& password); 
	static void AutoGetResource(HWND hTargetHwnd, CString url);
	//static bool IsQueueEmpty();
	//static bool IsDownloading(); 
	static void SendApi(APIRequest& request); // gui va chờ nhận
	// mot ham de khoi tao ,
	// ham de queue luon tu hoat dong 
	static UINT NetworkThreadHandle(LPVOID pParam);
	static std::condition_variable QueueCV;
private:
	static std::queue<APIRequest> ApiQueue;
	static std::mutex QueueMutex; 
	// luôn lock mutex khi truy cap

	// Đánh thức thread nền khi có request mới trong queue
	static bool StopThread;

	//static std::queue<bool> downloading; 
	//static std::mutex downloadingMutex; 

	//static std::condition_variable downloadingQueueCV; 




};


// {} tạo  1 scope ( phạm vi ) tạm thời để đảm bảo lock_guard được hủy vca2 unlock mutext trước khi gọi queuCVno

// QueueCV.notify_one() : đanh thức thread

