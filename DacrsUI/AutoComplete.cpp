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

// 安装 hook. 初始化控制 ID 和基于类名的控制类型

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

// 扫描串数组中匹配的文本，并添加匹配项到一个新数组，返回匹配项的数目。

// 对于编辑控制而言, 只需要找到第一个串，在这里使用了一个BOOL参数。

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

// 这个虚拟函数的参数是两个字符串，如果有匹配则返回 TRUE. 缺省情况下实现普通的

// 前缀比较-你可以重载它，例如，在忽略掉两个串中的‘www’字符。

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

// "hook" 函数截取发送到编辑控制和组合框的消息.我只对 EN_CHANGE 或

// CBN_EDITCHANGE感兴趣: 编辑控制的内容已经改变.

//

LRESULT CAutoComplete::WindowProc(UINT msg, WPARAM wp, LPARAM lp)

{
	
	if ((msg==WM_COMMAND && LOWORD(wp)==m_idMyControl) &&
		
		((m_iType==Edit && HIWORD(wp)==EN_CHANGE) ||
		
		(m_iType==ComboBox && HIWORD(wp)==CBN_EDITCHANGE))) {
		
		// 因为我要改变控制的内容，它将触发更多的EN_CHANGE消息，
		
		// 当我获得控制时使用 m_bIgnoreChangeMsg 结束处理.
		
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

// 这是实现输入完成的主函数.

//

void CAutoComplete::OnComplete(CWnd* pWnd, CString s)

{
	
	CStringArray arMatches; // 匹配串
	GetMatches(s,arMatches);
	if (s.GetLength() && arMatches.GetSize()&&m_iType)
		
		DoCompletion(pWnd, s, arMatches);
	
	m_sPrevious=s; // 记住当前串
	
}

void CAutoComplete::DoCompletion(CWnd* pWnd, CString s,
								 const CStringArray& arMatches)
								 
{
	
	if (m_iType==ComboBox)
	{
		
		// 这种强制转换从技术上讲是不正确的，但它是一个标准的MFC诀窍.
		
		CComboBox* pComboBox = (CComboBox*)pWnd;
		
		// 更新下拉框以反映可能的匹配
		
		pComboBox->ResetContent();

		
		for (int i=0; i <arMatches.GetSize();i++)
			
			pComboBox->AddString(arMatches[i]);
		
		// 用户箭头光标，这样用户才能选择
		
		::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
		
		// 显示下拉框
		
		pComboBox->ShowDropDown();
		
		pComboBox->SetWindowText(IgnoreCompletion(s) ? s : arMatches[0]);
		
		pComboBox->SetEditSel(s.GetLength(),-1);  //设置高亮
		
	} 
	else if (m_iType==Edit && !IgnoreCompletion(s)) 
	{
		
		// 这种强制转换从技术上讲是不正确的，但它是一个标准的MFC诀窍.
		
		CEdit* pEdit = (CEdit*)pWnd;
		
		pEdit->SetWindowText(arMatches[0]);
		
		pEdit->SetSel(s.GetLength(),-1);
		
	}

}

////////////////////////////////////////////////////////////////////////

// 当用户按下Backspace键删除敲入的字符时，这个函数用于关闭输入完成特性。

// 这时，当前的串将匹配最后输入的（前一个）串。在这种情况下,输入完成特性

// 是被屏蔽掉的。例如，如果用户敲如‘foo’,程序将自动完成‘foobar’,字符

// ‘bar’是高亮，如果用户按下Backspace键或者Delete键删除‘bar’时，程序

// 不会再次完成foobar，而是保留‘foo’,即便是用户继续按Backspace键也一样。

// 这个函数是我写的唯一一个说明比代码本身还长函数:)！

//

//

BOOL CAutoComplete::IgnoreCompletion(CString s)

{
	
	return s==m_sPrevious.Left(s.GetLength());
	
}


