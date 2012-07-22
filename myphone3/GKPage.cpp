///////////////////////////////////////////////////////////////////////////////
// GKPage.cpp : implementation file for Gatekeeper Settings Dialog class
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
#include "GKPage.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGKPage property page

IMPLEMENT_DYNCREATE(CGKPage, CPropertyPage)

CGKPage::CGKPage() : CPropertyPage(CGKPage::IDD)
{
	//{{AFX_DATA_INIT(CGKPage)
	m_gkid = _T("");
	m_gkrec = FALSE;
	m_gkuse = FALSE;
	m_gkdiscover = -1;
	m_gkhost = _T("");
	m_gkpass = _T("");
	//}}AFX_DATA_INIT

	this->m_psp.dwFlags^=PSP_HASHELP;

	// making image list for aliases
	imList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 0, 1);
	imList.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_USERSICON)));
}

CGKPage::~CGKPage()
{
}

void CGKPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGKPage)
	DDX_Control(pDX, IDC_ALSDELLBUTTON, m_aliasdelbutton);
	DDX_Control(pDX, IDC_ALSADDBUTTON, m_aliasaddbutton);
	DDX_Control(pDX, IDC_ALIASESLIST, m_aliaslist);
	DDX_Control(pDX, IDC_ALIASEDIT, m_alias);
	DDX_Control(pDX, IDC_GKAUTODISC, m_cgkautodesc);
	DDX_Control(pDX, IDC_GKHOST, m_cgkhost);
	DDX_Control(pDX, IDC_GKID, m_cgkid);
	DDX_Control(pDX, IDC_GKREC, m_cgkrec);
	DDX_Control(pDX, IDC_GKUSE, m_cgkuse);
	DDX_Text(pDX, IDC_GKID, m_gkid);
	DDX_Check(pDX, IDC_GKREC, m_gkrec);
	DDX_Check(pDX, IDC_GKUSE, m_gkuse);
	DDX_Radio(pDX, IDC_GKAUTODISC, m_gkdiscover);
	DDX_Text(pDX, IDC_GKHOST, m_gkhost);
	DDX_Text(pDX, IDC_GKPASSEDIT, m_gkpass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGKPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGKPage)
	ON_BN_CLICKED(IDC_GKUSE, OnGkuse)
	ON_BN_CLICKED(IDC_GKAUTODISC, OnGkautodisc)
	ON_BN_CLICKED(IDC_GKHOSTDISC, OnGkhostdisc)
	ON_BN_CLICKED(IDC_GKIDDISC, OnGkiddisc)
	ON_EN_CHANGE(IDC_ALIASEDIT, OnChangeAliasedit)
	ON_BN_CLICKED(IDC_ALSADDBUTTON, OnAlsaddbutton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ALIASESLIST, OnItemchangedAliaseslist)
	ON_BN_CLICKED(IDC_ALSDELLBUTTON, OnAlsdellbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGKPage message handlers

BOOL CGKPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_aliasaddbutton.EnableWindow(false);
	m_aliasdelbutton.EnableWindow(false);	
	CRect rect;
	m_aliaslist.GetClientRect(&rect);
	m_aliaslist.InsertColumn(0,_T(""),LVCFMT_LEFT,rect.Width(),0);
	m_aliaslist.SetImageList(&imList, LVSIL_SMALL);
	for (int i=0;i<m_AliasList.GetSize();i++)
	{
		m_aliaslist.InsertItem(i,m_AliasList.GetAt(i));
	}

	CtrlOnOff();
	//
	TranslateDlg();
	//
	return TRUE;
}

void CGKPage::CtrlOnOff(void)
{
	// disabling some controls
	UpdateData();
	m_cgkrec.EnableWindow(m_gkuse);
	m_cgkautodesc.EnableWindow(m_gkuse);
	GetDlgItem(IDC_GKHOSTDISC)->EnableWindow(m_gkuse);
	GetDlgItem(IDC_GKPASSSTATIC)->EnableWindow(m_gkuse);
	GetDlgItem(IDC_GKPASSEDIT)->EnableWindow(m_gkuse);
	GetDlgItem(IDC_GKIDDISC)->EnableWindow(m_gkuse);
	switch(m_gkdiscover)
	{
	case 0:
		m_cgkhost.EnableWindow(FALSE);	
		m_cgkid.EnableWindow(FALSE && m_gkuse);	
		break;
	case 1:
		m_cgkhost.EnableWindow(TRUE && m_gkuse);
		m_cgkid.EnableWindow(FALSE);			
		break;
	case 2:
		m_cgkhost.EnableWindow(FALSE);
		m_cgkid.EnableWindow(TRUE && m_gkuse);			
		break;
	}
}

void CGKPage::OnGkuse() 
{
	CtrlOnOff();	
}

void CGKPage::OnGkautodisc() 
{
	CtrlOnOff();		
}

void CGKPage::OnGkhostdisc() 
{
	CtrlOnOff();	
}

void CGKPage::OnGkiddisc() 
{
	CtrlOnOff();		
}

void CGKPage::OnChangeAliasedit() 
{
	CString AliasNew;
	m_alias.GetWindowText(AliasNew);

	m_aliasaddbutton.EnableWindow(!AliasNew.IsEmpty());
}

void CGKPage::OnAlsaddbutton() 
{
	CString AliasNew;
	m_alias.GetWindowText(AliasNew);
	AliasNew.TrimRight();
	AliasNew.TrimLeft();
	if(AliasNew.FindOneOf("=|")>=0) return;

	m_aliaslist.InsertItem(m_aliaslist.GetItemCount(),AliasNew);
	m_AliasList.Add(AliasNew);

	m_alias.SetWindowText(_T(""));

	OnChangeAliasedit();
	OnItemchangedAliaseslist(0, 0);
}

void CGKPage::OnItemchangedAliaseslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (pResult) *pResult = 0;

	POSITION pos = m_aliaslist.GetFirstSelectedItemPosition();
	m_aliasdelbutton.EnableWindow(pos!=NULL);

}

void CGKPage::OnAlsdellbutton() 
{
	POSITION pos = m_aliaslist.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int index = m_aliaslist.GetNextSelectedItem( pos );
	if (index<0 || index>=m_aliaslist.GetItemCount() ||
		m_aliaslist.GetItemCount()!=m_AliasList.GetSize())
		return;
	
	m_aliaslist.DeleteItem(index);
	m_AliasList.RemoveAt(index);

	OnItemchangedAliaseslist(0, 0);
}

void CGKPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_GKUSE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_GKUSESTR));
	GetDlgItem(IDC_GKREC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_GKRECSTR));
	GetDlgItem(IDC_GKSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_GKLOCSTR));
	GetDlgItem(IDC_GKAUTODISC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_GKAUTOLOCSTR));
	GetDlgItem(IDC_GKPASSSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_GKPASSSTR));
	GetDlgItem(IDC_GKHOSTDISC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_GKHOSTLOCSTR));	
	GetDlgItem(IDC_GKIDDISC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_GKIDLOCSTR));	
	GetDlgItem(IDC_UALIASESSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_UALIASESSTR));	
	GetDlgItem(IDC_UALEASSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_UALIASSTR));
	GetDlgItem(IDC_ALSADDBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_UALADDSTR));
	GetDlgItem(IDC_ALSDELLBUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_UALDELSTR));
//	SetWindowText((LPCTSTR)LoadStringLang(IDS_AUDIOSTR));	
}
