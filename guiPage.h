///////////////////////////////////////////////////////////////////////////////
// guiPage.h: interface for GUI Settings Dialog class
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

#if !defined(AFX_GUIPAGE_H__1EA01A25_81B5_41FA_BE26_BFD284F655C9__INCLUDED_)
#define AFX_GUIPAGE_H__1EA01A25_81B5_41FA_BE26_BFD284F655C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// guiPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CguiPage dialog

class CguiPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CguiPage)

// Construction
public:
	COLORREF clrInMsg;
	COLORREF clrOutMsg;
	COLORREF clrActInd;
	COLORREF clrSlndInd;
	CguiPage();
	~CguiPage();
	void TranslateDlg();

// Dialog Data
	//{{AFX_DATA(CguiPage)
	enum { IDD = IDD_GUIPAGE };
	BOOL	m_vidhidechk;
	BOOL	m_loghidechk;
	BOOL	m_stathidechk;
	BOOL	m_systraychk;
	BOOL	m_autoaddadrchk;
	int		m_lang;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CguiPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CguiPage)
	afx_msg void OnInmsgclr();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnOutmsgclr();
	afx_msg void OnActindclr();
	afx_msg void OnSlndindclr();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIPAGE_H__1EA01A25_81B5_41FA_BE26_BFD284F655C9__INCLUDED_)
