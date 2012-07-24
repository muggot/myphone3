///////////////////////////////////////////////////////////////////////////////
// GeneralPage.h: interface for General Settings Dialog class
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

#if !defined(AFX_GENERALPAGE_H__F70411EF_9CA0_40CC_941E_77B1F8F72CF9__INCLUDED_)
#define AFX_GENERALPAGE_H__F70411EF_9CA0_40CC_941E_77B1F8F72CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPage)

// Construction
public:
	CGeneralPage();
	~CGeneralPage();
	void TranslateDlg();

// Dialog Data
	//{{AFX_DATA(CGeneralPage)
	enum { IDD = IDD_GENERALPAGE };
	CEdit	m_RingFileCtrl;
	CButton	m_PlayBtn;
	CString		m_UserName;
	CString	m_RingSoundFile;
	BOOL	m_AutoAnswer;
	BOOL	m_NoTunneling;
	BOOL	m_DtmfAsString;
	BOOL	m_disableFastStart;
	BOOL	m_singleLine;

	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPage)
	afx_msg void OnRingFileBrowse();
	afx_msg void OnRingFilePlay();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeRingFileEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__F70411EF_9CA0_40CC_941E_77B1F8F72CF9__INCLUDED_)
