#include "pch.h"
#include "CListMessage.h"




BEGIN_MESSAGE_MAP(CListMessage, CListBox)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CListMessage::CListMessage() {}

CListMessage::~CListMessage() {}

BOOL CListMessage::Create(const CRect& rect, CWnd* pParent, UINT nId, CFont* font, CFont* fontDownload) {
    m_font = font; 
    m_fontDownload = fontDownload; 
    return CListBox::Create(WS_CHILD | WS_VISIBLE | LBS_OWNERDRAWVARIABLE |  WS_VSCROLL | LBS_HASSTRINGS | LBS_NOTIFY, rect, pParent, nId);
}


void CListMessage::AddItem(Entities::Message message)
{
    vt.push_back(message);
    //AfxMessageBox(message.link);
    AddString(message.content); 
}

void CListMessage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {


    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    int itemIndex = lpDrawItemStruct->itemID; // Tra ve index
    if (itemIndex < 0 || itemIndex >= vt.size())
    {
        return;
    }
   

    CRect rc = lpDrawItemStruct->rcItem;

    Entities::Message item = vt[itemIndex];
    CRect rDraw = rc;
    //pDC->RoundRect(rc, CPoint(10, 10)); 
    COLORREF colorBg;
    COLORREF colorText; 
    CRect cRect;
    int padding = 5;
    int d = 5; 
   
    CFont* pOldFont = pDC->SelectObject(m_font); 
    
    // WIDTH X HEIGHT = 800 X 600 
    // Item height : kich thuoc thuc te + kich thuoc padding + kich thuoc d , 
    // item.height = kich thuoc thuc te + (padding + d )* 2
    // tuong tu width cung vay
     if (item.messageType == 0) // YOU - bên trái
    {
        colorBg = GlobalParam::colorYou;
        colorText = GlobalParam::colorYouText;

        rDraw.left = rc.left + d + ( (rc.Width() / 20));
        rDraw.top = rc.top + d;
        rDraw.right = rDraw.left + item.width - ( 2 * d );
        rDraw.bottom = rDraw.top + item.height - ( 2 * d );

        // phu thuoc vao rDraw
        cRect.left = rDraw.left + padding ;
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
        rDraw.left = rc.right - item.width + d ;
        rDraw.bottom = rc.bottom - d;



        // phu thuoc vao rDraw
        // cRect : khung de ve chu, hoac cac phan noi dung 
        cRect.right = rDraw.right - padding ;
        cRect.top = rDraw.top + padding;
        cRect.left = rDraw.left + padding;
        cRect.bottom = rDraw.bottom - padding; 

    }

    //pDC->FillSolidRect(rDraw, color);
     
     CBrush brush(colorBg);
     CBrush* pOldBrush = pDC->SelectObject(&brush); 
     
     pDC->SelectStockObject(NULL_PEN); // khong the khoi phuc lai pen cu
     pDC->RoundRect(rDraw, CPoint(20, 20));


    //if (item.type == 0)
    //{
    //    CString str = vt[itemIndex].text;
    //    // ve chu;

    //}
    if (item.type == 0)
    {
        CString& str = vt[itemIndex].content;

        pDC->SetTextColor(colorText);
        pDC->SetBkMode(TRANSPARENT);

        DRAWTEXTPARAMS dtp = { sizeof(DRAWTEXTPARAMS) };

        ::DrawTextExW(
           pDC->GetSafeHdc(),
            str.GetBuffer(),
            -1,
            &cRect,
            DT_WORDBREAK | DT_EDITCONTROL, // KHÔNG dùng DT_CALCRECT ở đây!
            &dtp
        );
        str.ReleaseBuffer();
    }

    else if (item.type == 1)
    {

        CImage image;
        image.Load(item.link);
        image.Draw(pDC->GetSafeHdc(), cRect);
    }
    else if (item.type == 2)
    {

        HICON hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
        //pDC->DrawIcon(rDraw.left, rDraw.top, hIcon);
        pDC->SelectObject(m_fontDownload);
        ::DrawIconEx(pDC->GetSafeHdc(), cRect.left, cRect.top, hIcon, item.height - 20 , item.height - 20, 0, NULL, DI_NORMAL); 
        pDC->DrawText(item.content, CRect(cRect.left + (6 * ((item.height - 20) / 5 ) ) , cRect.top, cRect.right,cRect.bottom),DT_WORD_ELLIPSIS);
    }
    pDC->SelectObject(m_font); 

    int nextItemIndex = itemIndex + 1;  // ->>>>>>>>>
    if (nextItemIndex < 0 || nextItemIndex >= vt.size())
    {
        return;
    }
    Entities::Message nextItem = vt[nextItemIndex];
    if (nextItem.messageType == 1 && item.messageType == 0)
    {
        HICON hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
        ::DrawIconEx(pDC->GetSafeHdc(), cRect.left, cRect.top, hIcon, (rc.Width() / 25), (rc.Width() / 25), 0, NULL, DI_NORMAL);
    }
}




void CListMessage::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
    int index = lpMeasureItemStruct->itemID;
    if (index >= 0 && index < vt.size()) 
    {
        lpMeasureItemStruct->itemHeight = vt[index].height;
    }
    else 
    {
        lpMeasureItemStruct->itemHeight = 30;
    }
}
Entities::Message* CListMessage::GetMessageAt(int sel)
{
    if ( sel >= 0 && sel < vt.size())
        return &vt[sel];
    return nullptr;
}


void CListMessage::OnLButtonDown(UINT nFlags, CPoint point)
{
    BOOL bOutside = TRUE;
    int nIndex = ItemFromPoint(point, bOutside );

    if (!bOutside &&nIndex != LB_ERR)
    {
        SetCurSel(nIndex); 
        GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)m_hWnd);
    }


    CListBox::OnLButtonDown(nFlags, point); 
} 