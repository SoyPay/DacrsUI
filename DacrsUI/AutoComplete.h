// AutoComplete.h: interface for the CAutoComplete class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOCOMPLETE_H__ACFD1213_8492_4FED_966C_EBB7C47F5330__INCLUDED_)
#define AFX_AUTOCOMPLETE_H__ACFD1213_8492_4FED_966C_EBB7C47F5330__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma once

#include "Subclass.h"

// 只是个通用可重用类，你可以将它用于编辑框和组合框的自动完成输入应用.

//

// 使用方法:

// - 实例化你想要 hook 的编辑/组合控制

// - 添加一些串到串数组中

// - 调用 Init


class CAutoComplete  : public CSubclassWnd 
{
protected:
	CStringArray m_arStrings; // 串列表（数组）
	CString m_sPrevious; // 前一个内容
	int m_bIgnoreChangeMsg; // 忽略 EN_CHANGE 消息
	UINT m_idMyControl; // 子类化的控制 ID
	int m_iType; // 控制类型(编辑/组合)
	int m_iCurString; // 当前串的索引
	enum { Edit=1,ComboBox };
	
	// hook 函数
	
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
	
	// 可以重载的辅助函数
	
	virtual UINT GetMatches(LPCTSTR pszText, CStringArray& arMatches,
		BOOL bFirstOnly=FALSE);
	virtual void OnFirstString();
	virtual BOOL OnNextString(CString& sNext);
	virtual BOOL OnMatchString(const CString& s, const CString& sMatch);
	virtual BOOL IgnoreCompletion(CString s);
	virtual void OnComplete(CWnd* pWnd, CString s);
	virtual void DoCompletion(CWnd* pWnd, CString s,const CStringArray& arMatches);

public:
	CAutoComplete();
	virtual ~CAutoComplete();
	void Init(CWnd* pWnd);
	CStringArray& GetStringList() { return m_arStrings; }


};

#endif // !defined(AFX_AUTOCOMPLETE_H__ACFD1213_8492_4FED_966C_EBB7C47F5330__INCLUDED_)
