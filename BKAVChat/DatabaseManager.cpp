#include "pch.h"
#include "DatabaseManager.h"
DatabaseManager::DatabaseManager(){}

DatabaseManager::~DatabaseManager()
{
	Close(); 
}



DatabaseManager& DatabaseManager::GetInstance()
{ 
	static DatabaseManager instance; 
	return instance; 
}


bool DatabaseManager::Connect(BOOL isDropDb)
{
	if (m_db != nullptr)
		return true; 
	if (!isDropDb)
		if ( ::GetFileAttributes(FileManager::m_databasePath) != INVALID_FILE_ATTRIBUTES)
			CFile::Remove(FileManager::m_databasePath);
	int result = sqlite3_open(CW2A(FileManager::m_databasePath), &m_db);
	if (result == SQLITE_OK)
	{
		const char* query =
			"CREATE TABLE IF NOT EXISTS synced_friend ("
			" friendId TEXT PRIMARY KEY,"
			" lastSynced TEXT"
			");"
			"CREATE TABLE IF NOT EXISTS messages ("
			" id INTEGER PRIMARY KEY AUTOINCREMENT,"
			" friendId TEXT,"
			" type INTEGER,"
			" content TEXT,"
			" link TEXT,"
			" time TEXT,"
			" width integer default 0 ,"
			" height integer default 0, "
			" messageType integer, "
			" isSend integer"

			");";

		int rc = sqlite3_exec(m_db, query, 0, 0, nullptr);
	}

	return true;
}

sqlite3* DatabaseManager::GetConnection()
{
	return m_db; 
}

void DatabaseManager::Close()
{
	if (m_db)
	{
		sqlite3_close(m_db);
		m_db = nullptr;
	}
}
std::vector<Entities::Message> DatabaseManager::GetMessageFromLocal(CString friendId)
{
	
	std::vector<Entities::Message> ret;
	// them trường freindID
	const char* sql = "select type, content, link, time , width, height, messageType, isSend from messages where friendId = ? order by datetime(time) asc, id asc;";
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		CStringA friendIdA(friendId);
		sqlite3_bind_text(stmt, 1, friendIdA, -1, SQLITE_TRANSIENT);
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				Entities::Message msg; 
				msg.type = sqlite3_column_int(stmt, 0); 
				msg.content = CString(CA2T((const char*)(sqlite3_column_text(stmt, 1)), CP_UTF8));
				msg.link = CString(CA2T((const char*)(sqlite3_column_text(stmt, 2)), CP_UTF8));
				msg.time = 	CString(CA2T((const char*)(sqlite3_column_text(stmt, 3)), CP_UTF8)) ; 
				msg.width = sqlite3_column_int(stmt, 4);
				msg.height = sqlite3_column_int(stmt, 5);
				msg.messageType = sqlite3_column_int(stmt, 6);
				msg.isSend = sqlite3_column_int(stmt, 7);

				ret.push_back(msg); 
			}

	}
	sqlite3_finalize(stmt); 
	//CString format;
	//format.Format(_T("So tin nhan truy van ra : %d"), ret.size());
	//AfxMessageBox(format);
	return ret; 
}
CString DatabaseManager::CheckLastSynced(CString friendId)
{
	const char* query = "select lastSynced from synced_friend where friendId = ?;"; 
	sqlite3_stmt* stmt = nullptr; 
	if (sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr) == SQLITE_OK)
	{
		CStringA friendIdA(friendId); 
		sqlite3_bind_text(stmt, 1, friendIdA, -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			CString ret = CString(CA2T((const char*)(sqlite3_column_text(stmt, 0))));
			return ret;
		}
	}
	return _T(""); 
}
BOOL DatabaseManager::UpdateLastSynced(CString friendId, CString lastSynced)
{
	const char* query = "insert into synced_friend(friendId, lastSynced) values(?,?) on conflict(friendId) do update set lastSynced = excluded.lastSynced; ";
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr) == SQLITE_OK)
	{
		CStringA friendIdA(friendId);
		CStringA lastSyncedA(lastSynced); 
		sqlite3_bind_text(stmt, 1, friendIdA, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, lastSyncedA, -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) != SQLITE_DONE)
			return FALSE; 
	}
	return TRUE; 
}
BOOL DatabaseManager::InsertMessage(std::vector<Entities::Message> vt)
{
	const char* query = "insert into messages(friendId,type, content, link, time, width, height, messageType, isSend) values(?,?,?,?,?,?,?,?,? ); ";
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr) != SQLITE_OK)
		return FALSE;
	for (Entities::Message& msg : vt)
	{	
		sqlite3_clear_bindings(stmt); 
		CW2A friendUtf8(msg.friendId, CP_UTF8); 
		CW2A contentUtf8(msg.content, CP_UTF8); 
		CW2A linkUtf8(msg.link, CP_UTF8); 
		CW2A timeUtf8(msg.time, CP_UTF8); 
		sqlite3_bind_text(stmt, 1, friendUtf8, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 2, msg.type); 
		sqlite3_bind_text(stmt, 3, contentUtf8,-1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 4, linkUtf8, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 5, timeUtf8 ,-1, SQLITE_TRANSIENT);

		sqlite3_bind_int(stmt, 6, msg.width); 
		sqlite3_bind_int(stmt, 7, msg.height); 
		sqlite3_bind_int(stmt, 8, msg.messageType); 
		sqlite3_bind_int(stmt, 9, msg.isSend); 
		if (sqlite3_step(stmt) != SQLITE_DONE)
			return FALSE; 

		sqlite3_reset(stmt); 
	}
	
	sqlite3_finalize(stmt);
	return TRUE;

}
