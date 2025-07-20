#pragma once
#include <afxwin.h>
#include <deque>
#include "Entities.h"
#include "GlobalParam.h"
#include "resource.h"
#include "MessageHelper.h"
class CListMessage :
    public CWnd
{
public:
    CListMessage(); 
    ~CListMessage(); 
    BOOL Create(CRect rect, CWnd* pParent,UINT nID , CFont* font, CFont* fontDownload); 
    void PushBackItem(Entities::Message message);
    void PushFrontItem(Entities::Message message);
    void UpdateScrollBar(bool isScrollDown);
    BOOL SetScrollDown();
    Entities::Message GetMessageAtSel(int index); 

protected: 

    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    void PaintMessage(CDC* dc, Entities::Message& msg, int nIndex, CRect& rc, int& y);



    std::deque<Entities::Message> d_msg; 
    int  m_scrollPos;
    bool m_firstInitialize; 
    int m_currentTotalHeight = 0;
    CFont* m_font;
    CFont* m_fontDownload;
    DECLARE_MESSAGE_MAP(); 

};

