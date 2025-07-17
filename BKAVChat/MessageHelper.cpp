#include "pch.h"
#include "MessageHelper.h"



std::vector<Entities::Message> MessageHelper::Json2Message(std::string data, CWnd* cWnd, CString friendId, CFont* font)
{
	std::vector<Entities::Message> ret;
	nlohmann::json item; 
	item = nlohmann::json::parse(data); 
	std::string tmp = "";

	if (item.find("Content") != item.end())
	{
	
		Entities::Message message;
		message.friendId = friendId; 
		message.type = 0;
		tmp = item["Content"];
	/*	message.content = CString(CA2T(tmp.c_str()), CP_UTF8);*/
		
		int len = MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, nullptr, 0);
		if (len > 0)
		{
			std::wstring wstr(len - 1, 0);
			MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, &wstr[0], len);
			message.content = CString(wstr.c_str());
		}
		else
		{
			message.content = _T(""); // fallback nếu lỗi
		}
		CRect rect(0, 0, 360, 0);
		CClientDC dc(cWnd);
		CFont* pOldFont = dc.SelectObject(font); 
		DRAWTEXTPARAMS dtp = { sizeof(DRAWTEXTPARAMS) };
		::DrawTextExW(
			dc.GetSafeHdc(),                  // HDC
			message.content.GetBuffer(),        // LPTSTR (hoặc LPWSTR nếu Unicode)
			-1,                               // Độ dài chuỗi (hoặc -1 nếu null-terminated)
			&rect,                            // RECT*
			DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL,      // Format flags
			&dtp                              // DRAWTEXTPARAMS*
		);
		dc.SelectObject(pOldFont); 
		message.height = rect.Height() + 20;
		message.width = rect.Width() + 20;
		message.messageType = item["MessageType"];
		tmp = item["CreatedAt"];
		message.time = CString(CA2T(tmp.c_str()));
		if ( !message.content.IsEmpty())
			ret.push_back(message); 
	}
	if (item["Files"].size() != 0)
	{

		for (const auto& element : item["Files"])
		{
			Entities::Message message;
			message.friendId = friendId; 
			message.type = 2;
			tmp = element["FileName"];
			message.content = CString(CA2T(tmp.c_str()));
			// tra ve url file	
			tmp = element["urlFile"];
			message.link = CString(CA2T(tmp.c_str()));

			CClientDC dc(cWnd);
			CFont* pOldFont = dc.SelectObject(font); 

			CRect rect(0, 0, 360, 0);
			dc.DrawText(message.content, rect, DT_CALCRECT);
			dc.SelectObject(pOldFont); 
			message.height = rect.Height() + 20;
			message.width = rect.Width() +(6 * (rect.Height()) / 5) + 20;
			tmp = item["CreatedAt"];
			message.time = CString(CA2T(tmp.c_str()));
			message.messageType = item["MessageType"];
			ret.push_back(message);
		}
	}

	if (item["Images"].size() != 0)
	{
		for (const auto& element : item["Images"])
		{
			// Xu li sau khi tai ve anh thanh cong
			Entities::Message message;
			message.friendId = friendId; 
			message.type = 1;
			tmp = element["FileName"]; // luu ten anh
			message.content = CString(CA2T(tmp.c_str()));

			tmp = element["urlImage"];
			CString url = CString(CA2T(tmp.c_str()));
			// Kiem tra file co ton tai o local hay chua
			url.Replace('/', '\\');
			message.link = FileManager::m_path + url ;
			url.Replace('\\', '/'); 
			if (PathFileExists(message.link))
			{
				MessageHelper::AdjustmentSize(message);
			}
			else
			{
				APIHelper::AutoGetResource(cWnd->GetSafeHwnd(), url);
			}
			tmp = item["CreatedAt"];
			message.time = CString(CA2T(tmp.c_str()));
			message.messageType = item["MessageType"];
			ret.push_back(message); 
		}

		// Lay size
	}

	return ret; 
}
void MessageHelper::WaitResource(std::vector<Entities::Message>& param)
{
	for (Entities::Message& item : param)
	{
		if (item.type == 1)
		{
			if (item.height == 0)
			{
				MessageHelper::AdjustmentSize(item);
			}

		}
	}

}
void MessageHelper::AdjustmentSize(Entities::Message& message)
{
	CImage image;
	CString path = message.link; 
	int waited = 0;
	while (!PathFileExists(path) && waited < 5000)
	{
			Sleep(50);
			waited += 50;
	}
	image.Load(path);

	UINT width = image.GetWidth();
	UINT height = image.GetHeight(); 

	//Tinh toan kich thuoc cua anh
	if (width > 360) // 60% cua toan bo GetCientRect
	{
		double scale = static_cast<double>(width) / static_cast<double>(height);
		message.height = static_cast<UINT>(360 / scale) + 20;
		message.width = static_cast<UINT>(360.0) + 20;
	}
	else
	{
		message.height = image.GetHeight() + 20;
		message.width = image.GetWidth() + 20;
	}
}

