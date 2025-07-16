#include "pch.h"
#include "FileManager.h"
#include "json.hpp"
#include "fstream"
//sqlite3* FileManager::m_db = nullptr;
CString FileManager::m_databasePath = _T("");
CString FileManager::m_configPath = _T("");
CString FileManager::m_path = _T(""); 
BOOL FileManager::Init()
{
	TCHAR* appdata = nullptr;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appdata);

	CString path(appdata); // Tạo chuỗi CString từ chuỗi TCHAR  
	FileManager::m_path = path + _T("\\BKAVChat");
	FileManager::m_databasePath = path + _T("\\BKAVChat\\message.db"); 
	FileManager::m_configPath = path + _T("\\BKAVChat\\config.json");


	// tao BKAVChat, images, avatar
	CreateDirectory(path + _T("\\BKAVChat"), NULL);
	CreateDirectory(m_path + _T("\\images"), NULL);
	CreateDirectory(m_path + _T("\\images\\avatar"), NULL);

	CoTaskMemFree(appdata);

	// Neu chua co file config thi khoi tao 
	FileManager::InitConfig(); 
	
	return TRUE;
}


BOOL FileManager::DefaultConfig() {

	std::string string_path = std::string(CW2A(FileManager::m_configPath));
	std::ofstream file(string_path); 
	nlohmann::json config;
	config["username"] = "admin";
	config["password"] = "admin";
	//config["saveInfo"] = FALSE;  trong json khong su dung BOOL mà sử dụng boolean
	config["remember"] = false; 
	file << config.dump();
	file.close();
	return TRUE;
}
bool FileManager::InitConfig() 
{
	CString path = FileManager::m_configPath;
	std::string configPath = CW2A(path);
	std::ifstream checkFile(configPath); // Sử dụng ifstream để kiểm tra file đã tồn tại hay chưa
	if (checkFile.good())
	{
		// Kiểm tra file đã tồn tại hay chưa
		// Nếu đã tồn tại thì bỏ qua 
		checkFile.close(); 
		return TRUE;
	} else 
	{
		// Nếu file chưa được tạo thì tạo file mới
		// File đã được tạo thì ghi đè
		FileManager::DefaultConfig();
		return TRUE;

	}
	
}


CString FileManager::ReadConfig()
{
	std::string path = std::string(CT2A(FileManager::m_configPath));
	std::ifstream f(path);
	nlohmann::json j;
	f >> j;
	std::string result = j.dump();
	CString result_cstr = CString(CA2T(result.c_str()));
	
	return result_cstr;

}

BOOL FileManager::SaveConfig(CString username, CString password, bool remember)
{
	// Luu lai vao file Config  
	std::string path = CW2A(FileManager::m_configPath);		
	if (remember)
	{

		std::ofstream file(path);
		nlohmann::json config;
		// username va password phai la string
		// CString ( unicode ) to std::string std::string(CW2A(str));
		config["username"] = std::string(CW2A(username)); 
		config["password"] = std::string(CW2A(password));
		config["remember"] = true;
		file << config.dump();
		file.close();
	}
	else
	{
		FileManager::DefaultConfig();
	}
	return TRUE;
}

//void FileManager::CloseDatabase()
//{
//	if (FileManager::m_db != nullptr)
//	{
//		sqlite3_close(m_db);
//		m_db = nullptr;
//	}
//}
