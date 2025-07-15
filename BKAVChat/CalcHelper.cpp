#include "pch.h"
#include "CalcHelper.h"


CRect CalcHelper::CalcTextRect(CDC* dc, CString text, CFont* font)
{
	CRect rRet;
	//if (font != nullptr) 
	CFont* pOldFont = dc->SelectObject(font);
	dc->DrawText(text, &rRet, DT_CALCRECT);
	dc->SelectObject(pOldFont); 
	return rRet;
}