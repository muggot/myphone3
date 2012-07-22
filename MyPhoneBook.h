// MyPhoneBook.h
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

#if !defined(AFX_MYPHONEBOOK_H__6C386C3E_1F99_4FC3_97E5_735B7408CB75__INCLUDED_)
#define AFX_MYPHONEBOOK_H__6C386C3E_1F99_4FC3_97E5_735B7408CB75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPhoneBook.h : header file
//
#include "Book_AddressPage.h"
#include "Book_InPage.h"
#include "Book_MsdPage.h"
#include "Book_OutPage.h"

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneBook dialog

class CMyPhoneBook : public CDialog
{
// Construction
public:
	void TranslateDlg();
	void DeleteContactInfo(PString &curName, int iField);
	PString GetContactInfo(PString & curName, int iField);
	bool bCanModify;
	BOOL CopyContact(CString & oldName, CString & newName);
	CImageList * GetImageList();
	BOOL ModifyContact(CString & itmName, CString & itmAdress, CString & itmRing);
	BOOL DeleteOldContact(CString & itemKey, int iType=0);
	BOOL PrintItemFields(CString & itemKey);
	BOOL InsertNewContact(int iType=0, CString & newName=CString(_T("-new contact-")), CString & newAddress=CString(_T("-new address-")), CString & newRing=CString(_T("-default-")));
	PStringList GetContacts(int iType = 0);
	void SetBookTitle(CString & newTitle);
	CMyPhoneBook(CWnd* pParent = NULL);   // standard constructor


	CPropertySheet m_dlgPropSheet;
	CBook_AddressPage *adrPage;
	CBook_OutPage *outPage;
	CBook_InPage *inPage;
	CBook_MsdPage *msdPage;
	//
	PConfig adrbook;
	//
	CString curItemName;
	CString curItemAddress;
	CString curItemRing;
    //
	CImageList imList;
// Dialog Data
	//{{AFX_DATA(CMyPhoneBook)
	enum { IDD = IDD_BOOKDIALOG };
	CEdit	m_abookRingEdit;
	CEdit	m_abookAdrEdit;
	CEdit	m_abookNameEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPhoneBook)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyPhoneBook)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnAbookadrnameeditbutton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPHONEBOOK_H__6C386C3E_1F99_4FC3_97E5_735B7408CB75__INCLUDED_)
