// ButtonEx.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ButtonEx.h"

// CButtonEx

//IMPLEMENT_DYNAMIC(CButtonEx, CShadeButtonST)

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



// CButtonEx ���b�Z�[�W �n���h��



void CButtonEx::OnBnClicked()
{
	::SendMessage( m_hParent, WM_BN_CLICK, m_inItem, m_inSubItem );
}
