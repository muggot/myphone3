///////////////////////////////////////////////////////////////////////////////
// MyIndicator.h : Interface for my Level Indicator class
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

#if !defined(AFX_MYINDICATOR_H__CE34A604_2ABB_4B6A_A917_B241F5B7974E__INCLUDED_)
#define AFX_MYINDICATOR_H__CE34A604_2ABB_4B6A_A917_B241F5B7974E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyIndicator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyIndicator window

class CMyIndicator : public CStatic
{
// Construction
public:
	CMyIndicator();

protected:
    virtual UINT ScaleLevel(UINT level);
//    virtual void OnRedraw(PCanvas &);
    BOOL	logarithmic;
    BOOL	active;
    UINT	maximumLevel;
    UINT	markerPosition;  // These are always log scaled.
    UINT	currentPosition;
	CFrameWnd*	drawWnd;

public:
	static COLORREF	clrBg; 
	static COLORREF	clrFg; 
	static COLORREF	clrSFg; 

	void Reset();
	void SetMaximumLevel(UINT level);
	void SetActive(BOOL isActive);
	void SetMarkerLevel(UINT level);
	void SetCurrentLevel(UINT level);


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyIndicator)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyIndicator();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyIndicator)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYINDICATOR_H__CE34A604_2ABB_4B6A_A917_B241F5B7974E__INCLUDED_)
