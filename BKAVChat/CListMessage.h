#pragma once
#include <afxcmn.h>
#include <vector>
#include "GlobalParam.h"
#include "Entities.h"
#include "resource.h"
class CListMessage : public CListBox

{
    //
public:
    CListMessage();
    virtual ~CListMessage();
    BOOL Create(const CRect& rect, CWnd* pParent, UINT nId, CFont* font, CFont* fontDownload);
    void AddItem(Entities::Message message);
    Entities::Message* GetMessageAt(int sel);
protected:
    CFont* m_font; 
    CFont* m_fontDownload; 
    std::vector<Entities::Message> vt;
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


    
    
    DECLARE_MESSAGE_MAP()
};
