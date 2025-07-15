#pragma once
#include <afxwin.h>
class CCButton :
    public CButton
{
public :
    BOOL Create(LPCTSTR lpStr, CRect rect, CWnd* pParent, UINT nId, CFont* font, bool style);
    BOOL Create(HICON* hIcon, CRect rect, CWnd* pParent, UINT nId); 
protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point); 

    afx_msg LRESULT  OnMouseLeave(WPARAM wParam, LPARAM lParam); 
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); 

    HICON* m_hIcon = nullptr; 
    bool m_isTracking = false; 

    CFont* m_font = nullptr;
    COLORREF m_colorCurrent;
    COLORREF m_colorNormal; 
    COLORREF m_colorPressed;

    COLORREF m_colorText;
    bool m_style; 
    CBrush* m_backgroundBrush = CBrush::FromHandle(::GetSysColorBrush(COLOR_BTNFACE));
    DECLARE_MESSAGE_MAP()

};

