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

	// TODO:  添加您的代码以绘制指定项
CRect rect=lpDrawItemStruct->rcItem;
 CDC dc;
 dc.Attach(lpDrawItemStruct->hDC);
 //dc.FillSolidRect(rect,RGB(0,166,170));
 dc.FillSolidRect(rect,RGB(255,255,255));
 CFont Font;
 Font.CreatePointFont(90,"宋体");//创建字体
 dc.SelectObject(&Font);
 CString *pText=(CString *)lpDrawItemStruct->itemData;
 if(lpDrawItemStruct->itemState&ODS_SELECTED)
   dc.FillSolidRect(rect,RGB(80,89,202));//菜单被选中
// dc.SetTextColor(RGB(10,0,181));//设置文本颜色
 dc.SetTextColor(RGB(41,57,85));
 dc.DrawText(*pText,rect,DT_VCENTER|DT_LEFT|DT_SINGLELINE);
 dc.Detach();
}


void CNewMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight=25;//项高
	lpMeasureItemStruct->itemWidth=60;//项宽
}
void CNewMenu::ChangeMenuItem(CMenu *pMenu)
{
	int itemCount=pMenu->GetMenuItemCount();
	for(int i=0;i<itemCount;i++)
	{
		CString *pText=new CString;
		UINT itemID=pMenu->GetMenuItemID(i);//获取菜单项ID号
		pMenu->GetMenuString(i,*pText,MF_BYPOSITION);//获取菜单文本

		//ModifyMenu函数最后一个参数对应DRAWITEMSTRUCT结构里的itemData变量
		pMenu->ModifyMenu(i,MF_OWNERDRAW|MF_BYPOSITION|MF_STRING,itemID,(LPSTR)pText);
		if(itemID==-1)//如果是一个弹出式菜单
		{
			ChangeMenuItem(pMenu->GetSubMenu(i));
		}
	}  
}