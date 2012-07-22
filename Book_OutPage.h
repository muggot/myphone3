// Book_OutPage.h
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

#if !defined(AFX_BOOK_OUTPAGE_H__A1C7D628_8E1C_448F_BBA6_F2443BB383DA__INCLUDED_)
#define AFX_BOOK_OUTPAGE_H__A1C7D628_8E1C_448F_BBA6_F2443BB383DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Book_OutPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBook_OutPage dialog

class CBook_OutPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBook_OutPage)

// Construction
public:
	void TranslateDlg();
	void InitPropertyPage(CWnd *wndOwner);
	void BuildList();
	CBook_OutPage();
	~CBook_OutPage();

	CWnd *wndOwner;
	int m_curSelItem;
// Dialog Data
	//{{AFX_DATA(CBook_OutPage)
	enum { IDD = IDD_BOOK_OUT };
	CButton	m_clrButton;
	CButton	m_addButton;
	CListCtrl	m_contactsListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBook_OutPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBook_OutPage)
	afx_msg void OnItemchangedAbookoutlist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnAbookoutclrbutton();
	afx_msg void OnAbookadroutbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//	static int CALLBACK ListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOK_OUTPAGE_H__A1C7D628_8E1C_448F_BBA6_F2443BB383DA__INCLUDED_)
