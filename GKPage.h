///////////////////////////////////////////////////////////////////////////////
// GKPage.h: interface for Gatekeeper Settings Dialog class
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

#if !defined(AFX_GKPAGE_H__44CE1B35_5D23_4BA7_B4DB_ADAA0369FD6A__INCLUDED_)
#define AFX_GKPAGE_H__44CE1B35_5D23_4BA7_B4DB_ADAA0369FD6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGKPage dialog

class CGKPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGKPage)

// Construction
public:
	CStringArray  m_AliasList;
	CGKPage();
	~CGKPage();
	void TranslateDlg();

// Dialog Data
	//{{AFX_DATA(CGKPage)
	enum { IDD = IDD_GKPAGE };
	CButton	m_aliasdelbutton;
	CButton	m_aliasaddbutton;
	CListCtrl	m_aliaslist;
	CEdit	m_alias;
	CButton	m_cgkautodesc;
	CEdit	m_cgkhost;
	CEdit	m_cgkid;
	CButton	m_cgkrec;
	CButton	m_cgkuse;
	CString	m_gkid;
	BOOL	m_gkrec;
	BOOL	m_gkuse;
	int		m_gkdiscover;
	CString	m_gkhost;
	CString	m_gkpass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGKPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CtrlOnOff(void);
	CImageList imList;
	// Generated message map functions
	//{{AFX_MSG(CGKPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnGkuse();
	afx_msg void OnGkautodisc();
	afx_msg void OnGkhostdisc();
	afx_msg void OnGkiddisc();
	afx_msg void OnChangeAliasedit();
	afx_msg void OnAlsaddbutton();
	afx_msg void OnItemchangedAliaseslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAlsdellbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GKPAGE_H__44CE1B35_5D23_4BA7_B4DB_ADAA0369FD6A__INCLUDED_)
