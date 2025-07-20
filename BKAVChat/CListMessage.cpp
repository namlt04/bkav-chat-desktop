#include "pch.h" 
#include "CListMessage.h"

CListMessage::CListMessage() {}; 
CListMessage::~CListMessage() {}; 

BEGIN_MESSAGE_MAP(CListMessage, CWnd)
    ON_WM_PAINT()
    ON_WM_VSCROLL()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_MOUSEWHEEL() 
    ON_WM_LBUTTONDOWN()
    
END_MESSAGE_MAP() 

void CListMessage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL };
    GetScrollInfo(SB_VERT, &si);

    int pos = m_scrollPos;

    switch (nSBCode)
    {
    case SB_LINEUP:      pos -= 20; break;
    case SB_LINEDOWN:     pos += 20; break;
    case SB_PAGEUP:      pos -= si.nPage; break;
    case SB_PAGEDOWN:     pos += si.nPage; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:    pos = nPos; break;
    }

    pos = max(si.nMin, min(pos, si.nMax - si.nPage + 1));

    if (pos != m_scrollPos)
    {
        m_scrollPos = pos;
        SetScrollPos(SB_VERT, pos);
        Invalidate(); 
    }
    CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}
BOOL CListMessage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    int scrollAmount = (zDelta > 0) ? -20 : 20;

    SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL };
    GetScrollInfo(SB_VERT, &si);

    int newPos = m_scrollPos + scrollAmount;

    newPos = max(si.nMin, min(newPos, si.nMax - si.nPage + 1));

    if (newPos != m_scrollPos)
    {
        m_scrollPos = newPos;
        SetScrollPos(SB_VERT, m_scrollPos);
        Invalidate();
    }

    return TRUE; 

}
BOOL CListMessage::Create(CRect rect, CWnd* pParent, UINT nId,CFont* font, CFont* fontDownload)
{
    m_font = font; 
    m_fontDownload = fontDownload;
	return CWnd::Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParent, nId); 
}


void CListMessage::PushFrontItem(Entities::Message message)
{
    // Load tin nhan cu hon
    d_msg.push_front(message); 
    m_currentTotalHeight += message.height;
    
    UpdateScrollBar(false); 
    Invalidate(); 
}

void CListMessage::PushBackItem(Entities::Message message)

{
    CRect rect; 
    GetClientRect(&rect); 
    CTime t1 = MessageHelper::Convert2CTime(_T("2004-01-01T00:00:00.000Z"));
    CTime t2 = MessageHelper::Convert2CTime(message.time); 
    if (!d_msg.empty())
        t1 = MessageHelper::Convert2CTime((d_msg.back()).time);

    CTimeSpan diff = t2 - t1; 
    if (diff.GetHours() >= 12) {
        m_currentTotalHeight += 16; 
        // In ra thoi gian T2
    }
    

    d_msg.push_back(message); 
    m_currentTotalHeight += message.height;
    UpdateScrollBar(false); 
    Invalidate(); 
}
void CListMessage::UpdateScrollBar(bool isScrollDown) 
{
    CRect rc;
    GetWindowRect(&rc);
    
    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = max(0, m_currentTotalHeight - 1);
    si.nPage = max(1, rc.Height());

    if (isScrollDown)
    {
        m_scrollPos = max(si.nMin, si.nMax - (int)si.nPage + 1);
        UpdateWindow(); 
    }
    si.nPos = m_scrollPos;
   
    SetScrollInfo(SB_VERT, &si, TRUE);
    SetScrollPos(SB_VERT, m_scrollPos);
    Invalidate();  // Vẽ lại nếu cần
}





void CListMessage::OnPaint()
{
    CPaintDC dc(this);         // DC thật
    CRect client;
    GetClientRect(&client);

    // 1. Tạo DC bộ nhớ (offscreen)
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    // 2. Tạo bitmap tạm
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, client.Width(), client.Height());

    // 3. Gán bitmap vào DC bộ nhớ
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 4. Vẽ nền và nội dung lên DC bộ nhớ
    memDC.FillSolidRect(client, RGB(255, 255, 255));

    int y = -m_scrollPos;
    for (int i = 0 ;  i < d_msg.size() ; i++) 
    {
        Entities::Message msg = d_msg[i];
        CRect rect(0, y  , client.Width(), y + msg.height);
        if (rect.bottom >= client.top &&  rect.top <= client.bottom)
        {
            PaintMessage(&memDC, msg, i, rect, y); 
        }
        y += msg.height;
    }

    // 5. Blit từ DC bộ nhớ ra màn hình
    dc.BitBlt(0, 0, client.Width(), client.Height(), &memDC, 0, 0, SRCCOPY);

    // 6. Dọn dẹp
    memDC.SelectObject(pOldBitmap);
}
void CListMessage::PaintMessage(CDC* dc, Entities::Message& item, int nIndex, CRect &rc, int& y )
{

    if (nIndex == 0)
    {
        rc.MoveToY(rc.top + 16); 
    }
    CRect rDraw  = rc;
    //pDC->RoundRect(rc, CPoint(10, 10)); 
    COLORREF colorBg;
    COLORREF colorText;
    CRect cRect;
    int padding = 5;
    int d = 5;

    CFont* pOldFont = dc->SelectObject(m_font);

    // WIDTH X HEIGHT = 800 X 600 
    // Item height : kich thuoc thuc te + kich thuoc padding + kich thuoc d , 
    // item.height = kich thuoc thuc te + (padding + d )* 2
    // tuong tu width cung vay
    if (nIndex == 0)
    {
        dc->SetTextColor(RGB(130, 130, 130));
        dc->SetBkMode(TRANSPARENT);
        CTime t2 = MessageHelper::Convert2CTime(item.time);
        dc->DrawText(t2.Format(_T("%Y-%m-%d %H:%M")), CRect(0, y, rc.right, y + 16 ), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        y += 16;
    }
        
    if (item.messageType == 0) // YOU - bên trái
    {
        colorBg = GlobalParam::colorYou;
        colorText = GlobalParam::colorYouText;

        rDraw.left = rc.left + d + ((rc.Width() / 20));
        rDraw.top = rc.top + d;
        rDraw.right = rDraw.left + item.width - (2 * d);
        rDraw.bottom = rDraw.top + item.height - (2 * d);

        // phu thuoc vao rDraw
        cRect.left = rDraw.left + padding;
        cRect.top = rDraw.top + padding;
        cRect.right = rDraw.right - padding;
        cRect.bottom = rDraw.bottom - padding;
    }
    else // ME - bên phải
    {
        colorBg = GlobalParam::colorMe;
        colorText = GlobalParam::colorMeText;
        rDraw.right = rc.right - d;
        rDraw.top = rc.top + d;
        rDraw.left = rc.right - item.width + d;
        rDraw.bottom = rc.bottom - d;



        // phu thuoc vao rDraw
        // cRect : khung de ve chu, hoac cac phan noi dung 
        cRect.right = rDraw.right - padding;
        cRect.top = rDraw.top + padding;
        cRect.left = rDraw.left + padding;
        cRect.bottom = rDraw.bottom - padding;

    }

    //pDC->FillSolidRect(rDraw, color);

    CBrush brush(colorBg);
    CBrush* pOldBrush = dc->SelectObject(&brush);

    dc->SelectStockObject(NULL_PEN); // khong the khoi phuc lai pen cu
    dc->RoundRect(rDraw, CPoint(20, 20));


     if (item.type == 0)
    {
        CString& str = item.content;

        dc->SetTextColor(colorText);
        dc->SetBkMode(TRANSPARENT);

        DRAWTEXTPARAMS dtp = { sizeof(DRAWTEXTPARAMS) };

        ::DrawTextExW(
            dc->GetSafeHdc(),
            str.GetBuffer(),
            -1,
            &cRect,
            DT_WORDBREAK | DT_EDITCONTROL, 
            &dtp
        );
        str.ReleaseBuffer();
    }

    else if (item.type == 1)
    {

        CImage image;
        image.Load(item.link);
        image.Draw(dc->GetSafeHdc(), cRect);
    }
    else if (item.type == 2)
    {

        HICON hIcon = AfxGetApp()->LoadIconW(IDI_FILEATTACH);
        dc->SelectObject(m_fontDownload);
        ::DrawIconEx(dc->GetSafeHdc(), cRect.left, cRect.top, hIcon, item.height - 20, item.height - 20, 0, NULL, DI_NORMAL);
        dc->DrawText(item.content, CRect(cRect.left + (6 * ((item.height - 20) / 5)), cRect.top, cRect.right, cRect.bottom), DT_WORD_ELLIPSIS);
    }
    // Ve nut xem

    if (nIndex == d_msg.size() - 1)
    {
        HICON h_iconStatus = nullptr; 
        UINT rx, ry;
        if (item.isSend == 0)
            h_iconStatus = AfxGetApp()->LoadIconW(IDI_SEEN); 
         else
            h_iconStatus = AfxGetApp()->LoadIconW(IDI_RECEIVE); 

        if (item.messageType == 0)
        {
            // Hoac la ve icon ( tuc la khong co avatar ) 
            HICON hIcon = AfxGetApp()->LoadIconW(IDI_AVATAR);
            ::DrawIconEx(dc->GetSafeHdc(), (rc.Width() / 90), rDraw.bottom - 32, hIcon, 32, 32, 0, NULL, DI_NORMAL);
            rx = rDraw.left; 
            ry = rDraw.bottom; 

        }
        else
        {
            rx = rDraw.right - 16; 
            ry = rDraw.bottom; 
        }

        ::DrawIconEx(dc->GetSafeHdc(),rx, ry , h_iconStatus, 16, 16,0, NULL, DI_NORMAL); 
    }


    int nextItemIndex = nIndex + 1;  
    if (nextItemIndex >= 0 && nextItemIndex < d_msg.size())
    {
        Entities::Message nextItem = d_msg[nextItemIndex];
        if (nextItem.messageType == 1 && item.messageType == 0)
        {
            HICON hIcon = AfxGetApp()->LoadIconW(IDI_AVATAR);
            ::DrawIconEx(dc->GetSafeHdc(), (rc.Width() / 90), rDraw.bottom - 32, hIcon, 32, 32, 0, NULL, DI_NORMAL);
        }    
        
        // Ve thoi gian
        CTime t1 = MessageHelper::Convert2CTime(item.time);
        CTime t2 = MessageHelper::Convert2CTime(nextItem.time);

        CTimeSpan diff = t2 - t1;
        if (diff.GetHours() >= 12) {
            if (item.messageType == 0)
            {
                HICON hIcon = AfxGetApp()->LoadIconW(IDI_AVATAR);
                ::DrawIconEx(dc->GetSafeHdc(), (rc.Width() / 60), rDraw.bottom - 32, hIcon, 32, 32, 0, NULL, DI_NORMAL);
            }
            dc->SetTextColor(RGB(130, 130, 130)); 
            dc->SetBkMode(TRANSPARENT); 
            dc->DrawText(t2.Format(_T("%Y-%m-%d %H:%M")), CRect(0, rDraw.bottom, rc.right, rDraw.bottom + 16), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            y += 16;
        }

        
        
        
    }

    
    


    dc->SelectObject(m_font);
    
}
BOOL CListMessage::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; 
}
BOOL CListMessage::SetScrollDown()
{
    UpdateScrollBar(true); 
    return TRUE; 
}

void CListMessage::OnLButtonDown(UINT nFlags, CPoint point)
{

    CRect r; 
    GetClientRect(r); 
    int y = -m_scrollPos;
    for (int i = 0; i < d_msg.size(); i++)
    {
        Entities::Message msg = d_msg[i];

        CRect rect;
        if ( msg.messageType == 0 )
            rect = CRect(0, y, msg.width, y + msg.height);
        else 
            rect = CRect(r.right - msg.width, y, r.right, y + msg.height);


        if (rect.PtInRect(point))
        {
            GetParent()->PostMessageW(WM_USER + 800, i, 0); 
            break;
        }
        y += msg.height;
    }

    CWnd::OnLButtonDown(nFlags, point);
}

Entities::Message CListMessage::GetMessageAtSel(int index)
{
    return d_msg[index]; 
}




