#include "pch.h"
#include "CListFriend.h"


BEGIN_MESSAGE_MAP(CListFriend, CListBox)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, &CListFriend::OnMouseLeave)
END_MESSAGE_MAP()

CListFriend::CListFriend() 
{

}
CListFriend::~CListFriend()
{

}

BOOL CListFriend::Create(const CRect& rect, CWnd* pParent, UINT height,  UINT nId) 
{
	m_height = height/10; 
	m_colorCurrent = m_colorNormal; 
	m_fontName.CreateFont(
		m_height / 2,                    // Chiều cao font (pixel)
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
	m_fontContent.CreateFont(
		m_height / 3,                    // Chiều cao font (pixel)
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

	return CListBox::Create(WS_CHILD | WS_VISIBLE | LBS_OWNERDRAWFIXED | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS, rect, pParent, nId);
}
void CListFriend::AddItem(Entities::User user)
{
	if (user.avatar.IsEmpty()) 
		user.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME); 
	CString showName = user.fullname.IsEmpty() ? user.username : user.fullname;
	user.showName = showName;
	vt.push_back(user); 
	AddString(showName);
}

void CListFriend::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = m_height;
}
void CListFriend::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nIndex = lpDrawItemStruct->itemID;

	if (nIndex < 0 || nIndex >= GetCount())
		return;

	Entities::User user = vt[nIndex]; 
	(nIndex == m_hoverItem) ? m_colorCurrent = m_colorPressed : m_colorCurrent = m_colorNormal; 

	CString showName;
	GetText(nIndex, showName); 
	//HICON hIcon = icons[nIndex];
	CRect r = lpDrawItemStruct->rcItem; // r : rect cua rieng item do 
	pDC->FillSolidRect(&r, m_colorCurrent); 
	

	CRect rA(r.left, r.top, r.left + r.Height(), r.bottom) ;  // rect avatar 
	rA.DeflateRect(m_height / 10, m_height / 10 ); 
	//pDC->FillSolidRect(&rA, RGB(0, 0, 0)); 
	CRect rCA = rA;// rect Copy rA
	rCA.DeflateRect(rCA.Width() / 6, rCA.Width() / 6);
	pDC->Ellipse(&rA); 
	// In anh dai dien 
	if (user.hIcon != NULL)
	{
		::DrawIconEx(pDC->GetSafeHdc(), rCA.left, rCA.top, user.hIcon, rCA.Width(), rCA.Height(), 0, NULL, DI_NORMAL);
	}
	else {
		// load tu anh ra 
	}
	
	// In ra ten hien thi ( FullName neu FullName != "" : username ) 
	CFont* pOldFont = pDC->SelectObject(&m_fontName); 
	CRect rN(r.left ,r.top , r.right, r.top +  (m_height /2 )); 
	rN.MoveToXY( rA.right + (m_height / 10), rN.top + (m_height / 15));
	pDC->DrawText(showName, rN, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	// In ra noi dung tin nhan cuoi cung  (content ) 
	pDC->SelectObject(&m_fontContent);
	CRect rC(r.left,rN.bottom, r.right, rN.bottom + (m_height /3)); 
	rC.MoveToXY( rN.left, rC.top + (m_height / 15)) ; 
	pDC->DrawText(vt[nIndex].content, rC, DT_LEFT | DT_SINGLELINE); 
	pDC->SelectObject(pOldFont); 
}


void CListFriend::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint clientPoint = point; 
	BOOL outside; 
	int iHover = ItemFromPoint(clientPoint, outside);
	if (!m_isTracking)
	{
		TRACKMOUSEEVENT tme = { sizeof(tme) }; 
		tme.dwFlags = TME_LEAVE; 
		tme.hwndTrack = m_hWnd; 

		::TrackMouseEvent(&tme); 
		m_isTracking = true;
	
	}
	if (iHover != m_hoverItem)
	{
		m_hoverItem = iHover; 
		Invalidate();
	}

	return CListBox::OnMouseMove(nFlags, point); 
}
LRESULT CListFriend::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_isTracking = false; 
	m_hoverItem = -1;
	Invalidate(); 
	return 0; 
}


Entities::User CListFriend::GetUser(int nIndex)
{
	return vt[nIndex]; 
}