#pragma once
#include <string>
#include "GlobalParam.h"
#include "json.hpp"
#include "FileManager.h"
#include "APIHelper.h"
#include <vector>
#include "Entities.h"
class MessageHelper

{
public:

	static std::vector<Entities::Message> Json2Message(std::string data, CWnd* cWnd,CString friendId); 
	static void AdjustmentSize(Entities::Message& message);
	static void WaitResource(std::vector<Entities::Message>& param);
};

