// Book_MsdPage.h
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

#if !defined(AFX_BOOK_MSDPAGE_H__6CBCF557_2709_44F0_AF99_D622C71AF3CC__INCLUDED_)
#define AFX_BOOK_MSDPAGE_H__6CBCF557_2709_44F0_AF99_D622C71AF3CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Book_MsdPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBook_MsdPage dialog

class CBook_MsdPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBook_MsdPage)

// Construction
public:
	void TranslateDlg();
	void BuildList(void);
	void InitPropertyPage(CWnd *wndOwner);
	CBook_MsdPage();
	~CBook_MsdPage();

	CWnd *wndOwner;
	int m_curSelItem;
// Dialog Data
	//{{AFX_DATA(CBook_MsdPage)
	enum { IDD = IDD_BOOK_MISSED };
	CButton	m_clrButton;
	CButton	m_addButton;
	CListCtrl	m_contactsListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBook_MsdPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBook_MsdPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedAbookmsdlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAbookmsdclrbutton();
	afx_msg void OnAbookmsdaddbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOK_MSDPAGE_H__6CBCF557_2709_44F0_AF99_D622C71AF3CC__INCLUDED_)
