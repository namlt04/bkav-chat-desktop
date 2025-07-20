#pragma once
#include <vector>
#include "CCButton.h"
#include "resource.h"
#include "Entities.h"

class CChatView;
class CFilePicker : public CWnd
{
public:
    CFilePicker();
    ~CFilePicker();

    BOOL Create(CRect rect, CWnd* pParent, UINT nID);
    void AddAttach(bool isPickImage);  // chon them file
    void Close();  // Dong di bang chon
    std::vector<Entities::Attach>* GetFiles(); // Lay thong tin cua file da chon

protected:
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnAddButtonClicked(); 
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
    afx_msg void OnCloseBnClicked();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    void UpdateScrollBar(); // Cap nhat lai kich thuoc Horizontal ScrollBar

    DECLARE_MESSAGE_MAP()

    std::vector<Entities::Attach> files; 

    CFont m_font; 
    CCButton m_addButton;
    CCButton m_closeButton;
    HICON m_iconAdd; 
    HICON m_iconClose;
    HICON m_iconAttach; 

// Tu khoi tao
private:
    int m_scrollPos; // vi tri hien tai cua ScrollBar
    int m_width;
    int m_height;
    int m_spacing;
};

