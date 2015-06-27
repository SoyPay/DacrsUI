#include "StdAfx.h"
#include "NewMenu.h"


CNewMenu::CNewMenu(void)
{
}


CNewMenu::~CNewMenu(void)
{
}


void CNewMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  ������Ĵ����Ի���ָ����
CRect rect=lpDrawItemStruct->rcItem;
 CDC dc;
 dc.Attach(lpDrawItemStruct->hDC);
 //dc.FillSolidRect(rect,RGB(0,166,170));
 dc.FillSolidRect(rect,RGB(255,255,255));
 CFont Font;
 Font.CreatePointFont(90,"����");//��������
 dc.SelectObject(&Font);
 CString *pText=(CString *)lpDrawItemStruct->itemData;
 if(lpDrawItemStruct->itemState&ODS_SELECTED)
   dc.FillSolidRect(rect,RGB(80,89,202));//�˵���ѡ��
// dc.SetTextColor(RGB(10,0,181));//�����ı���ɫ
 dc.SetTextColor(RGB(41,57,85));
 dc.DrawText(*pText,rect,DT_VCENTER|DT_LEFT|DT_SINGLELINE);
 dc.Detach();
}


void CNewMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	// TODO:  ������Ĵ�����ȷ��ָ����Ĵ�С
	lpMeasureItemStruct->itemHeight=25;//���
	lpMeasureItemStruct->itemWidth=60;//���
}
void CNewMenu::ChangeMenuItem(CMenu *pMenu)
{
	int itemCount=pMenu->GetMenuItemCount();
	for(int i=0;i<itemCount;i++)
	{
		CString *pText=new CString;
		UINT itemID=pMenu->GetMenuItemID(i);//��ȡ�˵���ID��
		pMenu->GetMenuString(i,*pText,MF_BYPOSITION);//��ȡ�˵��ı�

		//ModifyMenu�������һ��������ӦDRAWITEMSTRUCT�ṹ���itemData����
		pMenu->ModifyMenu(i,MF_OWNERDRAW|MF_BYPOSITION|MF_STRING,itemID,(LPSTR)pText);
		if(itemID==-1)//�����һ������ʽ�˵�
		{
			ChangeMenuItem(pMenu->GetSubMenu(i));
		}
	}  
}