// MyPhoneBook.cpp : implementation file
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
#include "MyPhoneDlg.h"
//
//#include "Book_AddressPage.h"
//#include "Book_InPage.h"
//#include "Book_MsdPage.h"
//#include "Book_OutPage.h"
//
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
// CMyPhoneBook dialog


CMyPhoneBook::CMyPhoneBook(CWnd* pParent /*=NULL*/)
	: CDialog(CMyPhoneBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyPhoneBook)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// Make PConfig  object
	adrbook = PConfig(PFilePath(".\\mphnbook.mph"), "-empty-");

	// making image list
	imList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 0, 1);
	imList.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PHONE16)));
	imList.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PHONE16OUT)));
	imList.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PHONE16IN)));
	imList.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PHONE16MSD)));
	imList.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_COMP16)));

	adrPage = new CBook_AddressPage();
	outPage = new CBook_OutPage();
	inPage = new CBook_InPage();
	msdPage = new CBook_MsdPage();

	bCanModify = true;
}


void CMyPhoneBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPhoneBook)
	DDX_Control(pDX, IDC_ABOOKADRRINGEDIT, m_abookRingEdit);
	DDX_Control(pDX, IDC_ABOOKADRADREDIT, m_abookAdrEdit);
	DDX_Control(pDX, IDC_ABOOKADRNAMEEDIT, m_abookNameEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPhoneBook, CDialog)
	//{{AFX_MSG_MAP(CMyPhoneBook)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ABOOKADRNAMEEDITBUTTON, OnAbookadrnameeditbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneBook message handlers

BOOL CMyPhoneBook::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Load other PhoneBook's Tabs 
	if(!adrPage || !outPage || !inPage || !msdPage) return false;
	m_dlgPropSheet.AddPage(adrPage);
    m_dlgPropSheet.AddPage(outPage);
    m_dlgPropSheet.AddPage(inPage);
    m_dlgPropSheet.AddPage(msdPage);
	//
	CRect clRect, tbRect;
	GetClientRect(&clRect);
	m_dlgPropSheet.Create(this, WS_CHILD | WS_VISIBLE | WS_TABSTOP , WS_EX_CONTROLPARENT);
    m_dlgPropSheet.SetWindowPos(NULL, clRect.left, clRect.top, clRect.Width(), clRect.Height()-120,
    SWP_NOZORDER /* | SWP_NOACTIVATE*/);
	CTabCtrl *TabCtrl = m_dlgPropSheet.GetTabControl();
	TabCtrl->ModifyStyle(TCS_MULTILINE, TCS_BUTTONS | TCS_SINGLELINE);
	TabCtrl->GetWindowRect(&tbRect);
	m_dlgPropSheet.GetClientRect(&clRect);
	TabCtrl->SetWindowPos(NULL, 0, 0, clRect.Width()-13 /*??? Somehow tabcontrol is wider then we want*/,
		tbRect.Height(), SWP_NOMOVE );

	m_dlgPropSheet.SetActivePage(msdPage);
	m_dlgPropSheet.SetActivePage(inPage);
	m_dlgPropSheet.SetActivePage(outPage);
	m_dlgPropSheet.SetActivePage(adrPage);

	adrPage->InitPropertyPage(this);
	outPage->InitPropertyPage(this);
	inPage->InitPropertyPage(this);
	msdPage->InitPropertyPage(this);

	m_dlgPropSheet.SetActivePage(adrPage);

	TranslateDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyPhoneBook::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (adrPage){adrPage->DestroyWindow(); delete adrPage;}
	if (outPage){outPage->DestroyWindow(); delete outPage;}
	if (inPage){inPage->DestroyWindow(); delete inPage;}
	if (msdPage){msdPage->DestroyWindow(); delete msdPage;}
}

void CMyPhoneBook::SetBookTitle(CString &newTitle)
{
	// Change AddressBook Title 
	CString mainTitle = LoadStringLang(IDS_ABOOKTITLESTR);
	SetWindowText((LPCTSTR)CString(mainTitle +" - "+newTitle));
}

PStringList CMyPhoneBook::GetContacts(int iType)
{
  // Get list of Names
  PStringList outList, allList = adrbook.GetSections();
  for(int ind=0; ind<allList.GetSize(); ind++)
	switch(iType)
	{
	  case 0: if(CString((LPCTSTR)allList[ind]).Left(8)!="-unknown") // All
	    outList.AppendString(allList[ind]);
      break; 
	  case 1: if(adrbook.HasKey(allList[ind], "Outgoing")) // Outgoing
		outList.AppendString(/*"["+adrbook.GetString(allList[ind], "Outgoing", "?")+"] "+*/allList[ind]);
      break; 
	  case 2: if(adrbook.HasKey(allList[ind], "Incoming")) // Incoming
	    outList.AppendString(/*"["+adrbook.GetString(allList[ind], "Incoming", "?")+"] "+*/allList[ind]);
      break; 
	  case 3: if(adrbook.HasKey(allList[ind], "Missed")) // Missed
	    outList.AppendString(/*"["+adrbook.GetString(allList[ind], "Missed", "?")+"] "+*/allList[ind]);
      break; 
	}
  return outList;

}

BOOL CMyPhoneBook::InsertNewContact(int iType, CString & newName, CString & newAddress, CString & newRing)
{
	//Insert new Contact
	if(iType==0)
	{
    adrbook.SetString(PString((LPCTSTR)newName), "Name", PString((LPCTSTR)newName));
    adrbook.SetString(PString((LPCTSTR)newName), "Address", PString((LPCTSTR)newAddress));
	adrbook.SetString(PString((LPCTSTR)newName), "RingSound", PString((LPCTSTR)newRing));
	adrPage->BuildList();
	}
	else
	{
		if(iType==1 && newName!="-new contact-")
			adrbook.SetString(PString((LPCTSTR)newName), "Name", PString((LPCTSTR)newName));
		if(iType==2 && newAddress!="-new address-")
			adrbook.SetString(PString((LPCTSTR)newName), "Address", PString((LPCTSTR)newAddress));
		if(iType==3 && newRing!="-default-")
			adrbook.SetString(PString((LPCTSTR)newName), "RingSound", PString((LPCTSTR)newRing));
	}
	return TRUE;
}

BOOL CMyPhoneBook::PrintItemFields(CString &itemKey)
{
	// Set Item Data into Edit fields
	if(itemKey.IsEmpty()) return FALSE;
	curItemName = CString((LPCTSTR)adrbook.GetString(PString((LPCTSTR)itemKey), "Name", ""));
    curItemAddress = CString((LPCTSTR)adrbook.GetString(PString((LPCTSTR)itemKey), "Address", ""));
    curItemRing = CString((LPCTSTR)adrbook.GetString(PString((LPCTSTR)itemKey), "RingSound", ""));
    m_abookNameEdit.SetWindowText(curItemName);
	m_abookAdrEdit.SetWindowText(curItemAddress);
	m_abookRingEdit.SetWindowText(curItemRing);
    adrPage->m_abookSrchEdit.SetWindowText(curItemName+" ["+curItemAddress+"]");
	return TRUE;
}

BOOL CMyPhoneBook::DeleteOldContact(CString &itemKey, int iType)
{
	// Delete Old Contact by item Key));
	adrbook.DeleteSection((LPCTSTR)itemKey);
    m_abookNameEdit.SetWindowText("");
	m_abookAdrEdit.SetWindowText("");
	m_abookRingEdit.SetWindowText("");
	adrPage->m_abookSrchEdit.SetWindowText("");
	if(iType==0)
	{
		adrPage->BuildList();
		outPage->BuildList();
		inPage->BuildList();
		msdPage->BuildList();
	}
	return TRUE;
}

void CMyPhoneBook::OnAbookadrnameeditbutton() 
{
	// Enable Edit fields
    if(!m_abookNameEdit.IsWindowEnabled())
	{
		if(!bCanModify) return;
		m_abookNameEdit.EnableWindow();
		m_abookNameEdit.SetFocus();
		m_abookNameEdit.GetWindowText(curItemName);
		m_abookAdrEdit.EnableWindow();
		m_abookAdrEdit.GetWindowText(curItemAddress);
		bCanModify = false;
	}
	else
	{
		CString newName, newAddress, newRing;
		m_abookNameEdit.GetWindowText(newName);
		m_abookAdrEdit.GetWindowText(newAddress);
		m_abookRingEdit.GetWindowText(newRing);
		if((newName!="-new contact-") && (newName!=curItemName || newAddress!=curItemAddress) && !newName.IsEmpty())
		{
			BOOL isGood=TRUE;
			if(newAddress!=curItemAddress)	// Set new address
			{
				if(InsertNewContact(2, curItemName, newAddress))
					PrintItemFields(curItemName);
				else
					isGood=FALSE;
			}
			if(newName!=curItemName)
			{
				if(CopyContact(curItemName, newName) && 
					InsertNewContact(1, newName))
				{
					DeleteOldContact(curItemName, 1);
					adrPage->BuildList();
					outPage->BuildList();
					inPage->BuildList();
					msdPage->BuildList();
				}
				else
					isGood=FALSE;
			}
			if(isGood)
			{
	 			m_abookNameEdit.EnableWindow(FALSE);
				m_abookAdrEdit.EnableWindow(FALSE);
				bCanModify = true;
			}
		}
		else
		{
			m_abookNameEdit.SetWindowText(curItemName);
			m_abookNameEdit.EnableWindow(FALSE);
			m_abookAdrEdit.SetWindowText(curItemAddress);
			m_abookAdrEdit.EnableWindow(FALSE);
			bCanModify = true;
		}
	}
}

BOOL CMyPhoneBook::ModifyContact(CString &itmName, CString &itmAdress, CString &itmRing)
{
/*	// modify contact data 
	if(itemName.IsEmpty()) return FALSE;
    adrbook.DeleteSection(PString((LPCTSTR)itemKey));	
*/
	return TRUE;
}

void CMyPhoneBook::OnOK() 
{
	CString callName, callAdr;
	m_abookAdrEdit.GetWindowText(callAdr);
	m_abookNameEdit.GetWindowText(callName);
	if(callName.IsEmpty() || callAdr.IsEmpty()) return;
	((CMyPhoneDlg *)GetParent())->m_cdestination.SetWindowText(callAdr+" - "+callName);
	((CMyPhoneDlg *)GetParent())->UpdateData();
	((CMyPhoneDlg *)GetParent())->m_call.EnableWindow(
		(!((CMyPhoneDlg *)GetParent())->m_destination.IsEmpty()) && 
		(!((CMyPhoneDlg *)GetParent())->m_endpoint.HasConnection(((CMyPhoneDlg *)GetParent())->m_token)));	
	CDialog::OnOK();
}


CImageList * CMyPhoneBook::GetImageList()
{
	return imList.GetSafeHandle()!=NULL?&imList:NULL;
}

BOOL CMyPhoneBook::CopyContact(CString &oldName, CString &newName)
{
	// Copy contact with oldName to new contact with newName
   if(!adrbook.HasKey((LPCTSTR)oldName, "Name") ||
	   adrbook.HasKey((LPCTSTR)newName, "Name") ||
	   oldName==newName) return FALSE;
   PStringToString KeysValues = adrbook.GetAllKeyValues((LPCTSTR)oldName);
   for(int ind=0; ind<KeysValues.GetSize(); ind++)
	   adrbook.SetString((LPCTSTR)newName, KeysValues.GetKeyAt(ind), KeysValues.GetDataAt(ind));
   return TRUE;
}

PString CMyPhoneBook::GetContactInfo(PString &curName, int iField)
{
	// Get Contsct field data
	switch(iField)
	{
	default:
	case 0:
		return adrbook.GetString(curName, "Name", "-?-");
		break;
	case 1:
		return adrbook.GetString(curName, "Address", "-?-");
		break;
	case 2:
		return adrbook.GetString(curName, "RingSound", "-?-");
		break;
	case 3:
		return adrbook.GetString(curName, "Outgoing", "-?-");
		break;
	case 4:
		return adrbook.GetString(curName, "Incoming", "-?-");
		break;
	case 5:
		return adrbook.GetString(curName, "Missed", "-?-");
		break;
	}
}

void CMyPhoneBook::DeleteContactInfo(PString &curName, int iField)
{
	// delete contact field
	switch(iField)
	{
	case 3:
		adrbook.DeleteKey(curName, "Outgoing");
		break;
	case 4:
		adrbook.DeleteKey(curName, "Incoming");
		break;
	case 5:
		adrbook.DeleteKey(curName, "Missed");
		break;
	}
	// Deleting unknown contact if it has no outgoing, incoming or missed fields
	if(CString((LPCTSTR)adrbook.GetString(curName, "Name", "-no contact-")).Left(8)=="-unknown")
		DeleteOldContact(CString((LPCTSTR)curName));
}

void CMyPhoneBook::TranslateDlg()
{	// changing window languge
//	SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKTITLESTR));
	GetDlgItem(IDC_ABOOKADRNAMESTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRNAMESTR));
	GetDlgItem(IDC_ABOOKADRADRSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRADRSTR));
	GetDlgItem(IDC_ABOOKADRRINGSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKADRRINGSTR));	
	GetDlgItem(IDOK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_SBOOKCALLSTR));	
	// Translate TabedPane
	CTabCtrl* tab = m_dlgPropSheet.GetTabControl();
	for(int i=0; i<tab->GetItemCount(); i++)
	{
		CString title;
		TCITEM tabinfo;
		tabinfo.mask = TCIF_TEXT;
		switch(i)
		{
		case  0:
			title = LoadStringLang(IDS_ABOOKPBTITLESTR);
			break;
		case  1:
			title = LoadStringLang(IDS_ABOOKOUTTITLESTR);
			break;
		case  2:
			title = LoadStringLang(IDS_ABOOKINTITLESTR);
			break;
		case  3:
			title = LoadStringLang(IDS_ABOOKMSDTITLESTR);
			break;
		}
		if(title.IsEmpty()) continue;
		tabinfo.pszText = title.GetBuffer(0);
		tab->SetItem(i, &tabinfo);
	}
}

