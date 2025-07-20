#pragma once
#include <afxwin.h>
#include <vector>
#include "resource.h"
#include "Entities.h"
class CListFriend :
    public CListBox
{
public:
    CListFriend();
    virtual ~CListFriend(); 
    void AddItem(Entities::User user);
    BOOL Create(const CRect& rect, CWnd* pParent,UINT height,  UINT nId);
    Entities::User GetUser(int nIndex);
protected: 
    CArray<HICON> icons;
    std::vector<Entities::User> vt;
    UINT m_height; 
    afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    bool m_isTracking = false;
    int m_hoverItem = -1; 
    COLORREF m_colorCurrent;
    COLORREF m_colorNormal = RGB(255, 255, 255);
    COLORREF m_colorPressed = RGB(28, 127, 217);

    CFont m_fontName; 
    CFont m_fontContent; 

    DECLARE_MESSAGE_MAP();
};

