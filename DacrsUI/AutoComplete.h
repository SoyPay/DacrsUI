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

// ֻ�Ǹ�ͨ�ÿ������࣬����Խ������ڱ༭�����Ͽ���Զ��������Ӧ��.

//

// ʹ�÷���:

// - ʵ��������Ҫ hook �ı༭/��Ͽ���

// - ���һЩ������������

// - ���� Init


class CAutoComplete  : public CSubclassWnd 
{
protected:
	CStringArray m_arStrings; // ���б����飩
	CString m_sPrevious; // ǰһ������
	int m_bIgnoreChangeMsg; // ���� EN_CHANGE ��Ϣ
	UINT m_idMyControl; // ���໯�Ŀ��� ID
	int m_iType; // ��������(�༭/���)
	int m_iCurString; // ��ǰ��������
	enum { Edit=1,ComboBox };
	
	// hook ����
	
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
	
	// �������صĸ�������
	
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
