// Book_AddressPage.cpp : implementation file
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
#include "myphone.h"
#include "Book_AddressPage.h"
#include "MyPhoneBook.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBook_AddressPage property page

IMPLEMENT_DYNCREATE(CBook_AddressPage, CPropertyPage)

CBook_AddressPage::CBook_AddressPage() : CPropertyPage(CBook_AddressPage::IDD)
{
	wndOwner = NULL;
	//{{AFX_DATA_INIT(CBook_AddressPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBook_AddressPage::~CBook_AddressPage()
{
}

void CBook_AddressPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBook_AddressPage)
	DDX_Control(pDX, IDC_ABOOKADRDELBUTTON, m_clrButton);
	DDX_Control(pDX, IDC_ABOOKSRCHEDIT, m_abookSrchEdit);
	DDX_Control(pDX, IDC_ABOOKADRLIST, m_contactsListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBook_AddressPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBook_AddressPage)
	ON_BN_CLICKED(IDC_ABOOKADRADDBUTTON, OnAbookadraddbutton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ABOOKADRLIST, OnItemchangedAbookadrlist)
	ON_BN_CLICKED(IDC_ABOOKADRDELBUTTON, OnAbookadrdelbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBook_AddressPage message handlers

BOOL CBook_AddressPage::OnSetActive() 
{
	if(wndOwner!=NULL)
	  ((CMyPhoneBook *)wndOwner)->SetBookTitle(LoadStringLang(IDS_ABOOKPBTITLESTR));
	BuildList();
	
	return CPropertyPage::OnSetActive();
}

void CBook_AddressPage::InitPropertyPage(CWnd *wndOwner)
{
  this->wndOwner = wndOwner;
  if(wndOwner!=NULL && ((CMyPhoneBook *)wndOwner)->GetImageList()!=NULL)
    m_contactsListCtrl.SetImageList(((CMyPhoneBook *)wndOwner)->GetImageList(), LVSIL_SMALL);
}

void CBook_AddressPage::BuildList()
{
	//Build PhoneBook list
	if(wndOwner==NULL) return;
	m_curSelItem = (int)m_contactsListCtrl.GetFirstSelectedItemPosition();
	m_contactsListCtrl.DeleteAllItems();
	PSortedStringList list = PSortedStringList(((CMyPhoneBook *)wndOwner)->GetContacts()); 
	m_contactsListCtrl.SetItemCount(list.GetSize());
	for(int ind=0; ind<list.GetSize(); ind++)
      m_contactsListCtrl.InsertItem(ind, (LPCTSTR)list[ind], 0);
	if(m_curSelItem>0)
	  m_contactsListCtrl.SetItemState(m_curSelItem-1, LVIS_SELECTED, LVIS_SELECTED);
	if(list.GetSize()>0)
	  m_clrButton.EnableWindow(TRUE);
	else
	  m_clrButton.EnableWindow(FALSE);
}

void CBook_AddressPage::OnAbookadraddbutton() 
{
	// Call method for adding new contact
	if(wndOwner==NULL) return;
	((CMyPhoneBook *)wndOwner)->InsertNewContact();
	m_contactsListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

BOOL CBook_AddressPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    // configure ListCtrl
	CRect rect;
	m_contactsListCtrl.GetClientRect(&rect);
	m_contactsListCtrl.InsertColumn(0,_T(""),LVCFMT_LEFT,rect.Width(),0);
	//if(wndOwner!=NULL)
	//  m_contactsListCtrl.SetImageList(&(((CMyPhoneBook *)wndOwner)->imList), LVSIL_SMALL);

	TranslateDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBook_AddressPage::OnItemchangedAbookadrlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (pResult) *pResult = 0;
	
	// call method for getting new selected item data into Edit fields
	if(wndOwner==NULL) return;
    m_curSelItem = (int)m_contactsListCtrl.GetFirstSelectedItemPosition();
	if(m_curSelItem<=0) return;
	//CString cutItem = m_contactsListCtrl.GetItemText( ((int) pos)-1, 0);
    ((CMyPhoneBook *)wndOwner)->PrintItemFields(m_contactsListCtrl.GetItemText( ((int) m_curSelItem)-1, 0));
}

void CBook_AddressPage::OnAbookadrdelbutton() 
{
	// Call method for deleting curent contact
	if(wndOwner==NULL) return;
    POSITION pos = m_contactsListCtrl.GetFirstSelectedItemPosition();
	if(pos<=0) return;
	CString cutItem = m_contactsListCtrl.GetItemText( ((int) pos)-1, 0);
	if(AfxMessageBox("Are you sure?", MB_ICONEXCLAMATION | MB_YESNO)==IDYES)
      ((CMyPhoneBook *)wndOwner)->DeleteOldContact(cutItem);
//	BuildList();
}

void CBook_AddressPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_ABOOKADRADDBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRADDSTR));
	GetDlgItem(IDC_ABOOKADRDELBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRDELSTR));
}
