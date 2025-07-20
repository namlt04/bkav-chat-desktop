#include "pch.h"
#include "CCButton.h"



BEGIN_MESSAGE_MAP(CCButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, &CCButton::OnMouseLeave)
END_MESSAGE_MAP();

BOOL CCButton::Create(LPCTSTR lpText,CRect rect,CWnd* pParent, UINT nId, CFont* font,bool style)
{
	m_font = font; 
	m_style = style; 
	m_styleIcon = 0; 
	m_colorText = RGB(255, 255, 255);
	m_colorNormal = RGB(28, 127, 217);
	m_colorPressed = RGB(38, 123, 200);
	m_colorCurrent = m_colorNormal;
 
	return CButton::Create(lpText, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, rect, pParent, nId);
}


BOOL CCButton::Create(LPCTSTR lpText, HICON* hIcon, CRect rect, CWnd* pParent, UINT nId,UINT style)
{
 
	m_styleIcon = style;
	

	if (m_styleIcon == 3)
	{
		m_font = new CFont();
		m_font->CreateFont(
			64 ,                    // Chiều cao font (pixel)
			0,                     // Chiều rộng (0 = tự tính)
			0, 0,                  // Escapement, Orientation
			FW_NORMAL,               // Đậm
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
	}

	 
	m_hIcon = hIcon; 
	//m_colorNormal = RGB(255, 255, 255);
	m_colorNormal = ::GetSysColor(COLOR_BTNFACE);
	m_colorPressed = RGB(255, 255, 255); 
	m_colorCurrent = m_colorNormal;
	return CButton::Create(lpText, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, rect, pParent, nId);
}


void CCButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;
	CBrush brush(m_colorCurrent);
	CBrush* pOldBrush;
	if (m_styleIcon != 0 )
	{
		CPen pen(PS_NULL, 0, m_colorCurrent);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pOldBrush = pDC->SelectObject(&brush);
		if (m_styleIcon == 1 || m_styleIcon == 3) 
		{
			pDC->Ellipse(&rect);
			rect.DeflateRect(rect.Width() / 6 , rect.Width() / 6); 
		}
		if (m_styleIcon == 1 || m_styleIcon == 2)
			::DrawIconEx(pDC->GetSafeHdc(), rect.left, rect.top, *m_hIcon, rect.Width(), rect.Height(), 0, NULL, DI_NORMAL);

		pDC->SelectObject(pOldPen);
	}
	else
		if (m_style)
		{
			pOldBrush = pDC->SelectObject(&brush);
			pDC->SetTextColor(RGB(255,255,255));
			pDC->FillSolidRect(&rect, m_colorCurrent); 
		}
		else
		{
			pOldBrush = pDC->SelectObject(m_backgroundBrush); 
			pDC->SetTextColor(m_colorCurrent);
		}
	

	
	CString str;
	GetWindowText(str);
	CFont* pOldFont = nullptr; 
	if (m_font)
		pOldFont = pDC->SelectObject(m_font);
	if (!str.IsEmpty())
		pDC->DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldBrush);
	if (pOldFont)
		pDC->SelectObject(pOldFont);
};

void CCButton::OnMouseMove(UINT nFlags, CPoint point)
{

	if (!m_isTracking)
	{
		TRACKMOUSEEVENT tme = { sizeof(tme)};
		tme.dwFlags = TME_LEAVE; 
		tme.hwndTrack = m_hWnd;

		
		::TrackMouseEvent(&tme); 
		m_isTracking = true; 


		m_colorCurrent = m_colorPressed;
		Invalidate(); 

	}
	CButton::OnMouseMove(nFlags, point); 
}
LRESULT CCButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_isTracking = false;
	m_colorCurrent = m_colorNormal;
	Invalidate(); 
	return 0; 
}
