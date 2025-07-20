#pragma once
#include <vector>
#include "Entities.h"
#include "sqlite3.h"
#include "GlobalParam.h"
#include "FileManager.h"
class DatabaseManager
{
public : 
	DatabaseManager(); 
	~DatabaseManager(); 
	static DatabaseManager& GetInstance();
	CString CheckLastSynced(CString friendId);
	std::vector<Entities::Message> GetMessageFromLocal(CString friendId);
	BOOL InsertMessage(std::vector<Entities::Message> vt); 
	BOOL UpdateLastSynced(CString friendId, CString lastSynced); 
	bool Connect(BOOL isDropDb); 
protected: 
	


	sqlite3* GetConnection(); 

	void Close(); 
	sqlite3* m_db; 
		
};

