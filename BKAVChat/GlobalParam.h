#pragma once
#include <vector>
#include <map>
#include <queue>
#include "Entities.h"
struct Message {
	
	UINT type; // 0 : content, 1 : images, 2 : file
	// 0 neu la content thi text se chua content
	// 1 neu la images, text se chua link dan toi anh ( local ) 
	// 2 neu la file, text chua ten file 
	CString text;
	CString link; // click vao thi get image hoac anh
	UINT height;
	UINT width;
	CString time;
	UINT messageType; // la ban be nhan, 1 la ban than nhan

	// Khoi tao san
	Message(
		CString text = _T(""),
		CString link = _T(""),
		UINT height = 0,
		UINT width = 0,
		CString time = _T(""),
		UINT messageType = 0 
	) : text(text), link(link), height(height), width(width), time(time), messageType(messageType) { };
};
class GlobalParam
{
public:
	static CString token;
	static CString me;
	static CString current;
	static std::vector<CString> files;
	static std::map<CString, std::vector<Entities::Message>> messages;
	static std::queue<CString> queueKey; // luu tru key
	static COLORREF colorMe;
	static COLORREF colorYou;
	static COLORREF colorMeText;
	static COLORREF colorYouText;
};

