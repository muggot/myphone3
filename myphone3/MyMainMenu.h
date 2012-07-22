// MyMainMenu.h: interface for the CMyMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAINMENU_H__EF27365B_F3F9_425F_AB04_A231080C2EFD__INCLUDED_)
#define AFX_MYMAINMENU_H__EF27365B_F3F9_425F_AB04_A231080C2EFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyMainMenu : public CMenu  
{
public:
	CMyMainMenu();
	virtual ~CMyMainMenu();
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	void SetSpace(UINT iSpace);
	UINT GetSpace();

private:
	UINT mSpace;

};

#endif // !defined(AFX_MYMAINMENU_H__EF27365B_F3F9_425F_AB04_A231080C2EFD__INCLUDED_)
