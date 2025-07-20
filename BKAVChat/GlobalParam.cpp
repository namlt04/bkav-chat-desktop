#include "pch.h"
#include "GlobalParam.h"


CString GlobalParam::token = _T("");
CString GlobalParam::me = _T("admin");
CString GlobalParam::current = _T("");
//std::vector<CString> GlobalParam::files;
COLORREF GlobalParam::colorMe = RGB(67, 127, 236);

COLORREF GlobalParam::colorYou = RGB(218, 218, 218);
COLORREF GlobalParam::colorMeText = RGB(255, 255, 255);
COLORREF GlobalParam::colorYouText = RGB(0, 0, 0);
std::map<CString, std::vector<Entities::Message>> GlobalParam::messages;
std::queue<CString> GlobalParam::queueKey;

//if (queueKey.size() > 5)
//{
//	CString key = queueKey.front(); // lấy phần tử được thêm vào đầu tiên của hàng đợi 
//	queueKey.pop() // xóa phần tử đó  ra khỏi hàng đợi 
//	messages[key].clear();  // xóa vector chứa message của key
//	messages.erase(key); // xóa đi key đó gọi map 
//	
//}
//queueKey.push(key); 
//message[key] = vt;
