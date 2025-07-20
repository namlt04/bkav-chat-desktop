#include "pch.h"
#include "CFilePicker.h"

BEGIN_MESSAGE_MAP(CFilePicker,  CWnd)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_BN_CLICKED(7001, &CFilePicker::OnAddButtonClicked)
    ON_BN_CLICKED(7002, &CFilePicker::OnCloseBnClicked)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CFilePicker::CFilePicker()
: m_scrollPos(0),
m_width(100),
m_height(100),
m_spacing(10)
{}
CFilePicker::~CFilePicker(){}

void CFilePicker::OnLButtonDown(UINT nFlags, CPoint point)
{

    int x = - m_scrollPos + m_width + 2 * m_spacing; 
    for (int i = 0; i < files.size(); i++)
    {
        Entities::Attach attach = files[i]; 
        CRect rect(x, 10, x + attach.width, 10 + m_height); 
        if (rect.PtInRect(point))
        {
            files.erase(files.begin() + i);
            UpdateScrollBar(); 
            Invalidate(); 
            break;
        }
        x += m_width + m_spacing;
    }



    CWnd::OnLButtonDown(nFlags, point); 
}

BOOL CFilePicker::Create(CRect rect, CWnd* pParent, UINT nID)
{
    return CWnd::Create( NULL, NULL, WS_CHILD |WS_BORDER | WS_HSCROLL , rect, pParent, nID);
}

int CFilePicker::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    m_font.CreateFont(
        20 ,                    // Chiều cao font (pixel)
        0,                     // Chiều rộng (0 = tự tính)
        0, 0,                  // Escapement, Orientation
        FW_BOLD,               // Đậm
        FALSE,                 // Italic
        FALSE,                 // Underline
        0,                     // Strikeout
        ANSI_CHARSET,          // Charset (dùng ANSI nếu không cần Unicode đặc biệt)
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        _T("Segoe UI")         // Tên font
    );
    CRect rClient; 
    GetClientRect(&rClient); 
    m_iconAdd = AfxGetApp()->LoadIconW(IDI_ADD); 
    m_iconAttach = AfxGetApp()->LoadIconW(IDI_FILEATTACH);
    m_iconClose = AfxGetApp()->LoadIconW(IDI_CLOSE); 
    m_addButton.Create(_T(""),  & m_iconAdd, CRect(m_spacing, 10, m_spacing + m_width, m_spacing + m_height), this, 7001, 2);
    m_closeButton.Create(_T(""),  & m_iconClose, CRect(rClient.right - 30, rClient.top, rClient.right, rClient.top + 30), this, 7002, 1);
    UpdateScrollBar();
    return 0;
}

void CFilePicker::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    UpdateScrollBar();
}


void CFilePicker::UpdateScrollBar()
{
    CRect rc;
    GetClientRect(&rc);

    int totalWidth = 2 * m_spacing + m_width;
    for (Entities::Attach item : files)
        totalWidth += item.width + m_spacing; 

    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = max(0, totalWidth - 1);
    si.nPage = max(1, rc.Width() - 30);

    si.nPos = m_scrollPos;

    SetScrollInfo(SB_HORZ, &si, TRUE);
    SetScrollPos(SB_HORZ, m_scrollPos);
    Invalidate();  
}

void CFilePicker::OnPaint()
{
    CPaintDC paintDC(this);         // DC thật
    CRect client;
    GetClientRect(&client);

    // 1. Tạo DC bộ nhớ (offscreen)
    CDC memDC;
    memDC.CreateCompatibleDC(&paintDC);

    // 2. Tạo bitmap tạm
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&paintDC, client.Width(), client.Height());

    // 3. Gán bitmap vào DC bộ nhớ
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 4. Vẽ nền và nội dung lên DC bộ nhớ
    memDC.FillSolidRect(client, RGB(255, 255, 255));
    CFont* pOldFont = (CFont*)memDC.SelectObject(m_font); 
    int x = - ( m_scrollPos ) + m_width + 2 * m_spacing;
    for (int i = 0; i < files.size(); ++i)
    {
        Entities::Attach attach = files[i];
        CRect rect(x, 10, x + attach.width, 10 + attach.height);
        if (rect.right > 0 && rect.left < client.Width())
        {
            if (attach.type == _T("image"))
            {
                CImage img; 
                img.Load(attach.path); 
                img.Draw(memDC.GetSafeHdc(), rect); 
            }
            else 
            {
                
                ::DrawIconEx(memDC.GetSafeHdc(), rect.left, rect.top, m_iconAttach, m_width, m_height, 0,  NULL, DI_NORMAL);
                DRAWTEXTPARAMS dtp = { sizeof(dtp) }; 
                ::DrawTextEx(
                    memDC.GetSafeHdc(),
                    (attach.name).GetBuffer(),
                    -1,
                    CRect(rect.left + 100, rect.top, rect.right, rect.bottom),
                    DT_EDITCONTROL | DT_WORDBREAK ,
                    &dtp
                );
                (attach.name).ReleaseBuffer(); 
            }
            
        }
        x += attach.width + m_spacing;
    }
    // 5. Blit từ DC bộ nhớ ra màn hình
    paintDC.BitBlt(0, 0, client.Width(), client.Height(), &memDC, 0, 0, SRCCOPY);

    // 6. Dọn dẹp
    memDC.SelectObject(pOldBitmap);
}


void CFilePicker::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SCROLLINFO si = { sizeof(SCROLLINFO), SIF_ALL };
    GetScrollInfo(SB_HORZ, &si);

    int pos = m_scrollPos;

    switch (nSBCode)
    {
    case SB_LINELEFT:      pos -= 20; break;
    case SB_LINERIGHT:     pos += 20; break;
    case SB_PAGELEFT:      pos -= si.nPage; break;
    case SB_PAGERIGHT:     pos += si.nPage; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:    pos = nPos; break;
    }

    pos = max(si.nMin, min(pos, si.nMax - si.nPage + 1));

    if (pos != m_scrollPos)
    {
        m_scrollPos = pos;
        SetScrollPos(SB_HORZ, pos);
        Invalidate(); // Vẽ lại nội dung tại vị trí mới
        

        m_addButton.MoveWindow(CRect(- m_scrollPos + m_spacing,10, -  m_scrollPos + m_width + m_spacing, 10 + m_height));
    }

    CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CFilePicker::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // Tránh flicker
}

void CFilePicker::OnAddButtonClicked()
{
    AddAttach(false); 
}
void CFilePicker::AddAttach(bool isPickImage)
{
    LPCTSTR lpszFilter = _T("All Files (*.*)|*.*||");
    if (isPickImage)
    {
        lpszFilter = _T("Image Files (*.jpg;*.jpeg;*.png)|*.jpg;*.jpeg;*.png|All Files (*.*)|*.*||");
    }

    CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, lpszFilter, AfxGetMainWnd());

    if (dlg.DoModal() == IDOK)
    {
        CString path = dlg.GetPathName();

        Entities::Attach attach;
        CString ext = PathFindExtension(path);
        if (ext == _T(".jpg") || ext == _T(".jpeg") || ext == _T(".png"))
        {
            attach.type = _T("image");
            attach.name = PathFindFileName(path);
            attach.path = path;
            attach.width = 100;
            attach.height = 100; 
        }
        else
        {
            attach.type = _T("file");
            attach.name = PathFindFileName(path);
            attach.path = path; 
            attach.width = 150;
            attach.height = 100;
        }
        files.push_back(attach);
        UpdateScrollBar();
        Invalidate();
    }
}
void CFilePicker::OnCloseBnClicked()
{
    GetParent()->PostMessage(WM_USER + 500,0,0); 
}
void CFilePicker::Close()
{
    files.clear(); 
    ShowWindow(SW_HIDE); 
}
std::vector<Entities::Attach>* CFilePicker::GetFiles()
{
    return &files; 
}
