#pragma once
#include <string>
class FileManager
{

public : 
	static bool InitConfig();
	static CString ReadConfig();
	static BOOL Init();
	static BOOL DefaultConfig(std::string username);
	static BOOL SaveConfig(CString username, CString password, bool remember);
 
	static CString m_path; 
	static CString m_configPath; 
	static CString m_databasePath;
};

