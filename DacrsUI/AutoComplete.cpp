// AutoComplete.cpp: implementation of the CAutoComplete class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Edit.h"
#include "AutoComplete.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoComplete::CAutoComplete()
{
	
	m_bIgnoreChangeMsg=0;
	m_iType = 0;
	m_idMyControl = 0;
	m_iCurString = 0;
	
}

CAutoComplete::~CAutoComplete()

{
	
}

////////////////////////////////////////////////////////////////////////

// ��װ hook. ��ʼ������ ID �ͻ��������Ŀ�������

//

void CAutoComplete::Init(CWnd* pWnd)

{
	
	CSubclassWnd::HookWindow(pWnd->GetParent());
	
	CString sClassName;
	
	::GetClassName(pWnd->GetSafeHwnd(), sClassName.GetBuffer(32), 32);
	
	sClassName.ReleaseBuffer();
	
	if (sClassName=="Edit") {
		
		m_iType = Edit;
		
	} else if (sClassName=="ComboBox") {
		
		m_iType = ComboBox;
		
	}
	
	m_idMyControl = pWnd->GetDlgCtrlID();
	
}

//////////////////////////////////////////////////////////////////////////

// ɨ�贮������ƥ����ı��������ƥ���һ�������飬����ƥ�������Ŀ��

// ���ڱ༭���ƶ���, ֻ��Ҫ�ҵ���һ������������ʹ����һ��BOOL������

//

//

UINT CAutoComplete::GetMatches(LPCTSTR pszText, CStringArray& arMatches,
							   
							   BOOL bFirstOnly)
							   
{
	
	arMatches.RemoveAll();
	int nMatch = 0;
	CString s=pszText;
	if (s.GetLength()>0)
	{
		OnFirstString();
		CString sMatch;
		while (OnNextString(sMatch))
		{
			if (OnMatchString(s, sMatch))
			{
				TRACE("Add %s\n",(LPCTSTR)sMatch);
				arMatches.Add(sMatch);
				nMatch++;
				if (bFirstOnly)
					break;
			}
		}
	}
	return nMatch;
}

///////////////////////////////////////////////////////////////////////////

// ������⺯���Ĳ����������ַ����������ƥ���򷵻� TRUE. ȱʡ�����ʵ����ͨ��

// ǰ׺�Ƚ�-����������������磬�ں��Ե��������еġ�www���ַ���

//

//

BOOL CAutoComplete::OnMatchString(const CString& s, const CString& sMatch)

{	
	return s==sMatch.Left(s.GetLength());	
}

void CAutoComplete::OnFirstString()

{	
	m_iCurString=0;	
}

BOOL CAutoComplete::OnNextString(CString& sNext)

{	
	if (m_iCurString < m_arStrings.GetSize())
	{	
		sNext = m_arStrings[m_iCurString++];
		return TRUE;		
	}
	sNext = (LPCTSTR)NULL;
	
	return FALSE;	
}

////////////////////////////////////////////////////////////////////////

// "hook" ������ȡ���͵��༭���ƺ���Ͽ����Ϣ.��ֻ�� EN_CHANGE ��

// CBN_EDITCHANGE����Ȥ: �༭���Ƶ������Ѿ��ı�.

//

LRESULT CAutoComplete::WindowProc(UINT msg, WPARAM wp, LPARAM lp)

{
	
	if ((msg==WM_COMMAND && LOWORD(wp)==m_idMyControl) &&
		
		((m_iType==Edit && HIWORD(wp)==EN_CHANGE) ||
		
		(m_iType==ComboBox && HIWORD(wp)==CBN_EDITCHANGE))) {
		
		// ��Ϊ��Ҫ�ı���Ƶ����ݣ��������������EN_CHANGE��Ϣ��
		
		// ���һ�ÿ���ʱʹ�� m_bIgnoreChangeMsg ��������.
		
		if (!m_bIgnoreChangeMsg++) {
			
			CString s;
			
			CWnd* pWnd = CWnd::FromHandle((HWND)lp);
			
			pWnd->GetWindowText(s);
			
			OnComplete(pWnd, s);
			
		}
		
		m_bIgnoreChangeMsg--;
		
	}
	
	return CSubclassWnd::WindowProc(msg, wp, lp);
	
}

////////////////////////////////////////////////////////////////////////

// ����ʵ��������ɵ�������.

//

void CAutoComplete::OnComplete(CWnd* pWnd, CString s)

{
	
	CStringArray arMatches; // ƥ�䴮
	GetMatches(s,arMatches);
	if (s.GetLength() && arMatches.GetSize()&&m_iType)
		
		DoCompletion(pWnd, s, arMatches);
	
	m_sPrevious=s; // ��ס��ǰ��
	
}

void CAutoComplete::DoCompletion(CWnd* pWnd, CString s,
								 const CStringArray& arMatches)
								 
{
	
	if (m_iType==ComboBox)
	{
		
		// ����ǿ��ת���Ӽ����Ͻ��ǲ���ȷ�ģ�������һ����׼��MFC����.
		
		CComboBox* pComboBox = (CComboBox*)pWnd;
		
		// �����������Է�ӳ���ܵ�ƥ��
		
		pComboBox->ResetContent();

		
		for (int i=0; i <arMatches.GetSize();i++)
			
			pComboBox->AddString(arMatches[i]);
		
		// �û���ͷ��꣬�����û�����ѡ��
		
		::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
		
		// ��ʾ������
		
		pComboBox->ShowDropDown();
		
		pComboBox->SetWindowText(IgnoreCompletion(s) ? s : arMatches[0]);
		
		pComboBox->SetEditSel(s.GetLength(),-1);  //���ø���
		
	} 
	else if (m_iType==Edit && !IgnoreCompletion(s)) 
	{
		
		// ����ǿ��ת���Ӽ����Ͻ��ǲ���ȷ�ģ�������һ����׼��MFC����.
		
		CEdit* pEdit = (CEdit*)pWnd;
		
		pEdit->SetWindowText(arMatches[0]);
		
		pEdit->SetSel(s.GetLength(),-1);
		
	}

}

////////////////////////////////////////////////////////////////////////

// ���û�����Backspace��ɾ��������ַ�ʱ������������ڹر�����������ԡ�

// ��ʱ����ǰ�Ĵ���ƥ���������ģ�ǰһ�������������������,�����������

// �Ǳ����ε��ġ����磬����û����确foo��,�����Զ���ɡ�foobar��,�ַ�

// ��bar���Ǹ���������û�����Backspace������Delete��ɾ����bar��ʱ������

// �����ٴ����foobar�����Ǳ�����foo��,�������û�������Backspace��Ҳһ����

// �����������д��Ψһһ��˵���ȴ��뱾��������:)��

//

//

BOOL CAutoComplete::IgnoreCompletion(CString s)

{
	
	return s==m_sPrevious.Left(s.GetLength());
	
}


