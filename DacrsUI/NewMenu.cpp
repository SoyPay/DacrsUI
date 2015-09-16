#include "StdAfx.h"
#include "NewMenu.h"
#include "resource.h"

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
	 if(lpDrawItemStruct->itemState&ODS_SELECTED&&!(lpDrawItemStruct->itemState&ODS_GRAYED))
	   dc.FillSolidRect(rect,RGB(80,89,202));//菜单被选中
	 if (lpDrawItemStruct->itemState&ODS_GRAYED)
	 {
		 //   dc.FillSolidRect(rect,RGB(159,161,164));//菜单被禁用
			dc.SetTextColor(RGB(128,128,128));
	 }else{
		 dc.SetTextColor(RGB(41,57,85));
	 }
	 if (lpDrawItemStruct->itemState&ODS_CHECKED)
	 {
		 CBrush *brush = new CBrush;  
		 CPen *pen = new CPen;  
		 dc.SelectObject(pen);  
		 dc.SelectObject(brush); 
		 dc.MoveTo(rect.left+5, rect.top+10);
		 dc.LineTo(rect.left+10,rect.bottom-5);
		 dc.MoveTo(rect.left+10,rect.bottom-5);
		 dc.LineTo(rect.left+20,rect.top+5);
		 rect.left = rect.left+22;
		 CString showText=*pText;
		 showText.TrimLeft(" ");
		 dc.DrawText(showText,rect,DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		 dc.Detach();
		 delete brush;  
		 delete pen;  
		 return;
	 }
	

	 //if (lpDrawItemStruct->itemID == ID_RPC_CMD)
	 //{

		// CBitmap m_bitmap;

		// m_bitmap.LoadBitmap(IDB_BITMAP_ADDAPP_3);

		// BITMAP m_size;

		// m_bitmap.GetBitmap(&m_size);

		// CDC m_memdc;

		// m_memdc.CreateCompatibleDC(&dc);

		// CGdiObject* m_oldobject;

		// m_oldobject = m_memdc.SelectObject(&m_bitmap);

		// dc.StretchBlt(0,0,28,rect.bottom,&m_memdc,0,0,m_size.bmWidth,m_size.bmHeight,SRCCOPY);

		// m_bitmap.DeleteObject();
		// rect.left +=m_size.bmWidth ; 
	 //}
	 rect.left = rect.left+22;
	 CString showText=*pText;
	 showText.TrimLeft(" ");
	 dc.DrawText(showText,rect,DT_VCENTER|DT_LEFT|DT_SINGLELINE);
	 dc.Detach(); 
}


void CNewMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight=25;//项高
	//lpMeasureItemStruct->itemWidth=80;//项宽
	lpMeasureItemStruct->itemWidth=102;//项宽
	//lpMeasureItemStruct->itemWidth=120;//项宽
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
		//pMenu->ModifyMenu(i,MF_BYPOSITION|MF_STRING,itemID,(LPSTR)pText);
		if(itemID==-1)//如果是一个弹出式菜单
		{
			ChangeMenuItem(pMenu->GetSubMenu(i));
		}
	}  
}