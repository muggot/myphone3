///////////////////////////////////////////////////////////////////////////////
// Multilanguge.cpp : implementation file for Languge-aware functions
//
// MyPhone  -  Audio/Video IP telephony client.
// Compatible with H323 protocol, based on H323plus project.
// 
// Home Page: http://www.h323plus.org/
//
// Copyright (c) 2003 Michael Gerdov aka -=MaGGuS=-
// Copyright (c) 2007 ISVO (Asia) Pte Ltd.
//
// The contents of this file are subject to the Mozilla Public License
// Version 1.0 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
// the License for the specific language governing rights and limitations
// under the License.
// 
// Thanks goes to All OpenH323/H323plus members (www.h323plus.org)
//				  and especially to Yuriy Gorvitovskiy (PocketBone).
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPhone.h"
#include "Multilang.h"
//
extern int curLang;	// Curent Languge
/////////////////////////////////////////////////////////////////////////////
// Multilang aware functions implemention file
const DWORD tblLang[]={
	/*English (US)*/ MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
	/*French (FR)*/ MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH),
	/*Italian (IT)*/ MAKELANGID(LANG_ITALIAN, SUBLANG_ITALIAN),
	/*Russian*/ MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT)
};

CString LoadStringLang(UINT strID)
{
	// Loading String Table for curent languge
	LPCWSTR str=(LPCWSTR)LoadResourceLang(RT_STRING, 1+(strID >> 4));
	if (!str)
		return CString(_T("*N/A*"));
//    if(strID==IDS_RTPBSTR) //for debuging
//		int i=0;
	// Locate our string in the table
	for (WORD strPos=0; strPos < (strID & 0x000F); strPos++)
		str+=*str+1;

    if((*str)==0) // Zero String length
	{
		str=(LPCWSTR)LoadResourceLang(RT_STRING, 1+(strID >> 4), TRUE /*Safe English resource*/);
		if (!str)
			return CString(_T("*N/A*"));
		if(strID==IDS_RTPBSTR)
			int i=0;
		// Locate our string in the table
		for (WORD strPos=0; strPos < (strID & 0x000F); strPos++)
			str+=*str+1;
	}

	LPTSTR pDst = (LPTSTR)malloc(*str+1); // allocating buffer;
	// Copy to output buffer
#ifdef _UNICODE
	lstrcpyn(curStr.pDst, str+1, *str+1);
#else
	WideCharToMultiByte(CP_ACP, 0, str+1, *str+1, pDst, *str+1, NULL, NULL);
#endif
	pDst[*str]=TEXT('\0'); // Adding 0 at the end of the string
	CString pStr = CString(pDst); // Making string from buffer
	free(pDst); // freeing buffer
	return pStr;
}

HGLOBAL LoadResourceLang(LPCTSTR resType, DWORD resID, BOOL bSafeLang)
{
	// Loading Resource for curent languge
	HINSTANCE hAppInstance=GetModuleHandle(NULL);

	HRSRC hrRes = NULL;
	if(!bSafeLang)
	hrRes=FindResourceEx(hAppInstance, resType, MAKEINTRESOURCE(resID),
								curLang>0?curLang:GetUserDefaultLangID());
	if (!hrRes || bSafeLang)	// If not found then load English(US) as default
		hrRes=FindResourceEx(hAppInstance, resType, MAKEINTRESOURCE(resID),
								MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	return LoadResource(hAppInstance, hrRes);
}

DWORD GetLangFromTable(int langListID)
{
	if(langListID<0 || langListID>=sizeof(tblLang)/sizeof(tblLang[0]))
		return GetUserDefaultLangID();
	else
		return tblLang[langListID];
}