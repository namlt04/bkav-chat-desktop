#pragma once

#include "sqlite3.h"
class FileManager
{

public : 
	static bool InitConfig();
	static CString ReadConfig();
	static BOOL Init();
	static BOOL DefaultConfig();
	static BOOL SaveConfig(CString username, CString password, bool remember);
 
	static CString m_path; 
	static CString m_configPath; 
	static CString m_databasePath;
};

