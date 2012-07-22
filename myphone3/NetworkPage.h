///////////////////////////////////////////////////////////////////////////////
// NetworkPage.h: interface for Network Settings Dialog class
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

#if !defined(AFX_NETWORKPAGE_H__ED90959D_6313_4F1D_B585_258EF58F7ACA__INCLUDED_)
#define AFX_NETWORKPAGE_H__ED90959D_6313_4F1D_B585_258EF58F7ACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetworkPage dialog

class CNetworkPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CNetworkPage)

// Construction
public:
	CNetworkPage();
	~CNetworkPage();
	void TranslateDlg();

// Dialog Data
	//{{AFX_DATA(CNetworkPage)
	enum { IDD = IDD_NETWORKPAGE };
	CComboBox	m_BandwidthTypeCtrl;
	CEdit	m_BandwidthEdt;
	int		m_BandwidthType;
	double	m_Bandwidth;
	long	m_IP_Type;
	CString	m_linterface;
	CString	m_natip;
	UINT	m_rtpbase;
	UINT	m_rtpmax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNetworkPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNetworkPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBandwidthCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKPAGE_H__ED90959D_6313_4F1D_B585_258EF58F7ACA__INCLUDED_)
