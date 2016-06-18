#pragma once


// CStaticTrans
#define   TRANS_BACK   -1 

class CStaticTrans : public CStatic
{
	//   Construction 
public: 
        CStaticTrans(); 
        
protected: 
        COLORREF	m_TextColor; 
        COLORREF	m_BackColor; 
        CBrush       m_Brush; 
		CFont		*m_Font;
        
//   Operations 
public: 
        void   SetTextColor(COLORREF   col)   {   m_TextColor   =   col; 
                                                                            UpdateCtrl();             } 
        void   SetBackColor(COLORREF   col)   {   m_BackColor   =   col; 
                                                                            UpdateCtrl();             } 
		virtual void SetFont(int nHeight, LPCTSTR fontName, BOOL bRedraw = true);
private: 
        void   UpdateCtrl(); 
        
//   Overrides 
//   ClassWizard   generated   virtual   function   overrides 
//{{AFX_VIRTUAL(CStaticTrans) 
//}}AFX_VIRTUAL 

//   Implementation 
public: 
virtual   ~CStaticTrans(); 

//   Generated   message   map   functions 
protected: 
//{{AFX_MSG(CStaticTrans) 
afx_msg   HBRUSH   CtlColor(CDC*   pDC,   UINT   nCtlColor); 
//}}AFX_MSG 

DECLARE_MESSAGE_MAP() 

};


