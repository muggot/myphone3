///////////////////////////////////////////////////////////////////////////////
// Multilanguge.h : header file for Languge-aware functions
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

#if !defined(H_MULTILANG)
#define H_MULTILANG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

HGLOBAL LoadResourceLang(LPCTSTR resType, DWORD resID, BOOL bSafeLang=FALSE);
CString LoadStringLang(UINT strID);
DWORD GetLangFromTable(int langListID=-1);

#endif	//H_MULTILANG