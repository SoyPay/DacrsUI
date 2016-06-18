// StaticTrans.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "StaticTrans.h"


// CStaticTrans
CStaticTrans::CStaticTrans() 
{ 
        m_TextColor   =   RGB(0,   0,   0); 
        m_BackColor   =   TRANS_BACK;         
		m_Font = NULL ;
} 

CStaticTrans::~CStaticTrans() 
{ 
	if(m_Font)
	{
		m_Font->DeleteObject();
		delete m_Font;
	} 
} 


BEGIN_MESSAGE_MAP(CStaticTrans,   CStatic) 
//{{AFX_MSG_MAP(CStaticTrans) 
ON_WM_CTLCOLOR_REFLECT() 
//}}AFX_MSG_MAP 
END_MESSAGE_MAP() 

///////////////////////////////////////////////////////////////////////////// 
//   CStaticTrans   message   handlers 

HBRUSH   CStaticTrans::CtlColor(CDC*   pDC,   UINT   nCtlColor)   
{ 
        m_Brush.DeleteObject(); 
        if( m_BackColor == TRANS_BACK)   { 
			m_Brush.CreateStockObject(HOLLOW_BRUSH); 
			pDC->SetBkMode(TRANSPARENT); 
        } else { 
            m_Brush.CreateSolidBrush(m_BackColor); 
            pDC->SetBkColor(m_BackColor); 
        } 
		if(m_Font) {
			pDC->SelectObject(m_Font);
		}
        
        pDC->SetTextColor(m_TextColor); 
        
        return (HBRUSH)m_Brush;
} 

void   CStaticTrans::UpdateCtrl() 
{ 
        CWnd*   pParent   =   GetParent(); 
        CRect   rect; 
        
        GetWindowRect(rect); 
        pParent->ScreenToClient(rect); 
        rect.DeflateRect(2,   2); 
        
        pParent->InvalidateRect(rect,   FALSE);         
} 

void CStaticTrans::SetFont(int nHeight, LPCTSTR fontName, BOOL bRedraw)
{
  if(m_Font)
    m_Font->DeleteObject();

  if(m_Font == NULL)
    m_Font = new CFont();

  if(m_Font)
  {
    if(!m_Font->CreatePointFont(nHeight, fontName))
    {
      delete m_Font; 
      m_Font = NULL;
    }
  }
  if(bRedraw && GetSafeHwnd())
    Invalidate();
}




// CStaticTrans 消息处理程序


