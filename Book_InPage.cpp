// Book_InPage.cpp : implementation file
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
#include "Book_InPage.h"
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
static int CALLBACK ListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   // The lParam of an item is just its index.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(lParam1, 1);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, 1);

   return strcmp(strItem2, strItem1);
}

/////////////////////////////////////////////////////////////////////////////
// CBook_InPage property page

IMPLEMENT_DYNCREATE(CBook_InPage, CPropertyPage)

CBook_InPage::CBook_InPage() : CPropertyPage(CBook_InPage::IDD)
{
	wndOwner = NULL;
	//{{AFX_DATA_INIT(CBook_InPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBook_InPage::~CBook_InPage()
{
}

void CBook_InPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBook_InPage)
	DDX_Control(pDX, IDC_ABOOKINCLRBUTTON, m_clrButton);
	DDX_Control(pDX, IDC_ABOOKINADDBUTTON, m_addButton);
	DDX_Control(pDX, IDC_ABOOKINLIST, m_contactsListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBook_InPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBook_InPage)
	ON_NOTIFY(HDN_ITEMCHANGED, IDC_ABOOKINLIST, OnItemchangedAbookinlist)
	ON_BN_CLICKED(IDC_ABOOKINCLRBUTTON, OnAbookinclrbutton)
	ON_BN_CLICKED(IDC_ABOOKINADDBUTTON, OnAbookinaddbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBook_InPage message handlers

BOOL CBook_InPage::OnSetActive() 
{
	if(wndOwner!=NULL)
	  ((CMyPhoneBook *)wndOwner)->SetBookTitle(LoadStringLang(IDS_ABOOKINTITLESTR));
	BuildList();
	
	return CPropertyPage::OnSetActive();
}

void CBook_InPage::InitPropertyPage(CWnd *wndOwner)
{
  this->wndOwner = wndOwner;
  if(wndOwner!=NULL && ((CMyPhoneBook *)wndOwner)->GetImageList()!=NULL)
    m_contactsListCtrl.SetImageList(((CMyPhoneBook *)wndOwner)->GetImageList(), LVSIL_SMALL);
}

void CBook_InPage::BuildList()
{
	//Build PhoneBook list
	if(wndOwner==NULL) return;
	m_curSelItem = (int)m_contactsListCtrl.GetFirstSelectedItemPosition();
	m_contactsListCtrl.DeleteAllItems();
	PSortedStringList list = PSortedStringList(((CMyPhoneBook *)wndOwner)->GetContacts(2 /*Incoming*/)); 
	m_contactsListCtrl.SetItemCount(list.GetSize());
	for(int ind=0; ind<list.GetSize(); ind++)
	{
      m_contactsListCtrl.InsertItem(ind, (LPCTSTR)list[ind], 2);
	  m_contactsListCtrl.SetItemText(ind, 1, ((CMyPhoneBook *)wndOwner)->GetContactInfo(list[ind], 4));
	  m_contactsListCtrl.SetItemData(ind, ind);
	}
    // Sort the list view items using my callback procedure.
    BOOL bRes = m_contactsListCtrl.SortItems(ListCompareProc, (LPARAM) &m_contactsListCtrl);
	//
	if(m_curSelItem>0)
	  m_contactsListCtrl.SetItemState(m_curSelItem-1, LVIS_SELECTED, LVIS_SELECTED);
	if(list.GetSize()>0)
	  m_clrButton.EnableWindow(TRUE);
	else
	  m_clrButton.EnableWindow(FALSE);
}

BOOL CBook_InPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    // configure ListCtrl
	CRect rect;
	m_contactsListCtrl.GetClientRect(&rect);
	m_contactsListCtrl.InsertColumn(0,_T(""),LVCFMT_LEFT,rect.Width()/2,0);
	m_contactsListCtrl.InsertColumn(1,_T(""),LVCFMT_LEFT,rect.Width()/2,1);

	TranslateDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBook_InPage::OnItemchangedAbookinlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// call method for getting new selected item data into Edit fields
	if(wndOwner==NULL) return;
    m_curSelItem = (int)m_contactsListCtrl.GetFirstSelectedItemPosition();
	if(m_curSelItem<=0) return;
    CString curItem = m_contactsListCtrl.GetItemText( ((int) m_curSelItem)-1, 0);
	((CMyPhoneBook *)wndOwner)->PrintItemFields(curItem);
	if(curItem.Left(8)=="-unknown")
		m_addButton.EnableWindow(TRUE);
	else
		m_addButton.EnableWindow(FALSE);

	*pResult = 0;
}

void CBook_InPage::OnAbookinclrbutton() 
{
	// clear last incoming calls list
	if(AfxMessageBox("Are you sure?", MB_ICONEXCLAMATION | MB_YESNO)==IDYES)	
	{
		PStringList allCont = ((CMyPhoneBook *)wndOwner)->GetContacts(2);
		for(int ind=0; ind<allCont.GetSize(); ind++)
			((CMyPhoneBook *)wndOwner)->DeleteContactInfo(allCont[ind], 4);
		BuildList();
	}
}

void CBook_InPage::OnAbookinaddbutton() 
{
	// Add curent contact to PhoneBook if it is unknown
	if(wndOwner==NULL) return;
    POSITION pos = m_contactsListCtrl.GetFirstSelectedItemPosition();
	if(pos<=0) return;
	CString curItem = m_contactsListCtrl.GetItemText( ((int) pos)-1, 0);
	if(curItem.Left(8)=="-unknown")
	{
		CString newName = "-new contact "+curItem.Mid(9);
		if(((CMyPhoneBook *)wndOwner)->CopyContact(curItem, newName) && 
		   ((CMyPhoneBook *)wndOwner)->InsertNewContact(1, newName))
		{
			((CMyPhoneBook *)wndOwner)->DeleteOldContact(curItem);
		}
	}
}

void CBook_InPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_ABOOKINADDBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRSTORESTR));
	GetDlgItem(IDC_ABOOKINCLRBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRCLRSTR));
}
