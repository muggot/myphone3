// Book_MsdPage.cpp : implementation file
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
#include "Book_MsdPage.h"
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
// CBook_MsdPage property page

IMPLEMENT_DYNCREATE(CBook_MsdPage, CPropertyPage)

CBook_MsdPage::CBook_MsdPage() : CPropertyPage(CBook_MsdPage::IDD)
{
	wndOwner = NULL;
	//{{AFX_DATA_INIT(CBook_MsdPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBook_MsdPage::~CBook_MsdPage()
{
}

void CBook_MsdPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBook_MsdPage)
	DDX_Control(pDX, IDC_ABOOKMSDCLRBUTTON, m_clrButton);
	DDX_Control(pDX, IDC_ABOOKMSDADDBUTTON, m_addButton);
	DDX_Control(pDX, IDC_ABOOKMSDLIST, m_contactsListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBook_MsdPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBook_MsdPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ABOOKMSDLIST, OnItemchangedAbookmsdlist)
	ON_BN_CLICKED(IDC_ABOOKMSDCLRBUTTON, OnAbookmsdclrbutton)
	ON_BN_CLICKED(IDC_ABOOKMSDADDBUTTON, OnAbookmsdaddbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBook_MsdPage message handlers

BOOL CBook_MsdPage::OnSetActive() 
{
	if(wndOwner!=NULL)
	  ((CMyPhoneBook *)wndOwner)->SetBookTitle(LoadStringLang(IDS_ABOOKMSDTITLESTR));
	BuildList();
	
	return CPropertyPage::OnSetActive();
}

void CBook_MsdPage::InitPropertyPage(CWnd *wndOwner)
{
  this->wndOwner = wndOwner;
  if(wndOwner!=NULL && ((CMyPhoneBook *)wndOwner)->GetImageList()!=NULL)
    m_contactsListCtrl.SetImageList(((CMyPhoneBook *)wndOwner)->GetImageList(), LVSIL_SMALL);
}

BOOL CBook_MsdPage::OnInitDialog() 
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

void CBook_MsdPage::BuildList()
{
	//Build PhoneBook list
	if(wndOwner==NULL) return;
	m_curSelItem = (int)m_contactsListCtrl.GetFirstSelectedItemPosition();
	m_contactsListCtrl.DeleteAllItems();
	PSortedStringList list = PSortedStringList(((CMyPhoneBook *)wndOwner)->GetContacts(3/*Missed*/)); 
	m_contactsListCtrl.SetItemCount(list.GetSize());
	for(int ind=0; ind<list.GetSize(); ind++)
	{
      m_contactsListCtrl.InsertItem(ind, (LPCTSTR)list[ind], 3);
	  m_contactsListCtrl.SetItemText(ind, 1, ((CMyPhoneBook *)wndOwner)->GetContactInfo(list[ind], 5));
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


void CBook_MsdPage::OnItemchangedAbookmsdlist(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CBook_MsdPage::OnAbookmsdclrbutton() 
{
	// Clear last missed calls list
	if(AfxMessageBox("Are you sure?", MB_ICONEXCLAMATION | MB_YESNO)==IDYES)
	{
		PStringList allCont = ((CMyPhoneBook *)wndOwner)->GetContacts(3);
		for(int ind=0; ind<allCont.GetSize(); ind++)
			((CMyPhoneBook *)wndOwner)->DeleteContactInfo(allCont[ind], 5);
		BuildList();
	}
}

void CBook_MsdPage::OnAbookmsdaddbutton() 
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

void CBook_MsdPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_ABOOKMSDADDBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRSTORESTR));
	GetDlgItem(IDC_ABOOKMSDCLRBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRCLRSTR));
}
