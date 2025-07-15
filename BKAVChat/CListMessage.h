#pragma once
#include <afxcmn.h>
#include <vector>
#include "GlobalParam.h"
#include "Entities.h"
class CListMessage : public CListBox

{
    //
public:
    CListMessage();
    virtual ~CListMessage();
    BOOL Create(const CRect& rect, CWnd* pParent, UINT nId);
    void AddItem(Entities::Message message);
    Message* GetMessageAt(int sel);
protected:
    std::vector<Entities::Message> vt;
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);



    
    
    DECLARE_MESSAGE_MAP()
};
