// MyMainMenu.cpp: implementation of the CMyMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myphone.h"
#include "MyMainMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyMainMenu::CMyMainMenu()
{
	mSpace = 0;
}

CMyMainMenu::~CMyMainMenu()
{

}

void CMyMainMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// Make item wide
	int t1 = lpMeasureItemStruct->itemWidth;
	lpMeasureItemStruct->itemWidth = mSpace;
}

void CMyMainMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Do nothing
}
void CMyMainMenu::SetSpace(UINT iSpace)
{
	mSpace = iSpace;
}

UINT CMyMainMenu::GetSpace()
{
	return mSpace;
}
