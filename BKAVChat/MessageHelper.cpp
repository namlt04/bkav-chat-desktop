#include "pch.h"
#include "MessageHelper.h"



std::vector<Entities::Message> MessageHelper::Json2Message(std::string data, CWnd* cWnd)
{
	std::vector<Entities::Message> ret;
	nlohmann::json item; 
	item = nlohmann::json::parse(data); 
	std::string tmp = "";

	if (item.find("Content") != item.end())
	{
	
		Entities::Message message;
		message.type = 0;
		tmp = item["Content"];
		message.content = CString(CA2T(tmp.c_str()));
		CRect rect(0, 0, 360, 0);
		CClientDC dc(cWnd);
		//dc.DrawTextEx(message.text, rect, DT_WORDBREAK | DT_CALCRECT);
		DRAWTEXTPARAMS dtp = { sizeof(DRAWTEXTPARAMS) };
		::DrawTextExW(
			dc.GetSafeHdc(),                  // HDC
			message.content.GetBuffer(),        // LPTSTR (hoặc LPWSTR nếu Unicode)
			-1,                               // Độ dài chuỗi (hoặc -1 nếu null-terminated)
			&rect,                            // RECT*
			DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL,      // Format flags
			&dtp                              // DRAWTEXTPARAMS*
		);
		message.height = rect.Height() + 20;
		message.width = rect.Width() + 20;
		message.messageType = item["MessageType"];
		tmp = item["CreatedAt"];
		message.time = CString(CA2T(tmp.c_str()));
		ret.push_back(message); 
	}
	if (item["Files"].size() != 0)
	{

		for (const auto& element : item["Files"])
		{
			Entities::Message message;
			message.type = 2;
			tmp = element["FileName"];
			message.content = CString(CA2T(tmp.c_str()));
			// tra ve url file	
			tmp = element["urlFile"];
			message.link = CString(CA2T(tmp.c_str()));

			CClientDC dc(cWnd);
			CRect rect(0, 0, 360, 0);
			dc.DrawText(message.content, rect, DT_SINGLELINE | DT_CALCRECT);
			message.height = /* icon.hight() */ 32 + 20;
			message.width = 32 + 5 + rect.Width() + 20;
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
			message.type = 1;
			tmp = element["FileName"]; // luu ten anh
			message.content = CString(CA2T(tmp.c_str()));

			tmp = element["urlImage"];
			CString url = CString(CA2T(tmp.c_str()));
			LPCTSTR filename = PathFindFileName(url);
			// Kiem tra file co ton tai o local hay chua
			message.link = FileManager::m_imagesPath + _T("\\") + filename;
			if (PathFileExists(message.link))
			{
				MessageHelper::AdjustmentSize(message);
			}
			else
			{
				APIHelper::GetResource(cWnd->GetSafeHwnd(), url, _T("/images"));
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
	MSG msg;
	while (!APIHelper::IsQueueEmpty())
	{
		// Doi cho toi khi tai het anh thanh cong 
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(50);

	}
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
	image.Load(message.link);
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

