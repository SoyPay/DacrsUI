// ButtonEx.cpp : 実装ファイル
//
#include "stdafx.h"
#include "DacrsUI.h"
#include "afxdialogex.h"
#include "ButtonEx.h"


// CButtonEx

IMPLEMENT_DYNAMIC(CButtonEx, CShadeButtonST)

CButtonEx::CButtonEx()
{

}

CButtonEx::CButtonEx( int nItem, int nSubItem, CRect rect, HWND hParent )
{
	m_inItem = nItem;
	m_inSubItem = nSubItem;
	m_rect = rect;
	m_hParent = hParent;
	bEnable = TRUE;
}

CButtonEx::~CButtonEx()
{
}


BEGIN_MESSAGE_MAP(CButtonEx, CShadeButtonST)
	ON_CONTROL_REFLECT(BN_CLICKED, &CButtonEx::OnBnClicked)
END_MESSAGE_MAP()



// CButtonEx メッセージ ハンドラ



void CButtonEx::OnBnClicked()
{
	::SendMessage( m_hParent, WM_BN_CLICK, m_inItem, m_inSubItem );
}





IMPLEMENT_DYNAMIC(CButtonCtrl, CShadeButtonST)

CButtonCtrl::CButtonCtrl()
{

}

CButtonCtrl::CButtonCtrl( int nItem, int nSubItem, CRect rect, HWND hParent ,void*pData)
{
	m_inItem = nItem;
	m_inSubItem = nSubItem;
	m_rect = rect;
	m_hParent = hParent;
	bEnable = TRUE;
	memcpy(&m_pData,pData,sizeof(ADD_APP_DATA));
}
CButtonCtrl::~CButtonCtrl()
{
}


BEGIN_MESSAGE_MAP(CButtonCtrl, CShadeButtonST)
	ON_CONTROL_REFLECT(BN_CLICKED, &CButtonCtrl::OnBnClicked)
END_MESSAGE_MAP()



// CButtonCtrl ﾏ鋧｢



void CButtonCtrl::OnBnClicked()
{
	::SendMessage( m_hParent, WM_BN_CLICK, m_inItem, (LPARAM)this );
}