///////////////////////////////////////////////////////////////////////////////
// AudioPage.h: interface for Audio settings dialog class
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


#if !defined(AFX_AUDIOPAGE_H__0E380150_021B_4DC2_BBFF_F4EF17DCC038__INCLUDED_)
#define AFX_AUDIOPAGE_H__0E380150_021B_4DC2_BBFF_F4EF17DCC038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AudioPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAudioPage dialog

class CAudioPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAudioPage)

// Construction
public:
	CAudioPage();
	~CAudioPage();
	void TranslateDlg();

// Dialog Data
	//{{AFX_DATA(CAudioPage)
	enum { IDD = IDD_AUDIOPAGE };
	CSpinButtonCtrl	m_spinBuffers;
	CComboBox	m_PlayDeviceBox;
	CComboBox	m_RecordDeviceBox;
	CButton	m_UpBtn;
	CButton	m_EnableBtn;
	CButton	m_DownBtn;
	CListBox	m_CodecListCtrl;
	CString	m_PlayDeviceCur;
	CString	m_editBuffers;
	int		m_Jitter;
	CString	m_RecordDeviceCur;
	BOOL	m_SilenceDetect;
        BOOL	m_AECEnable;
        int	m_AGCEnable;
	//}}AFX_DATA
	CStringArray m_PlayDeviceList;
	CStringArray m_RecordDeviceList;
	CStringArray m_CodecList;
	unsigned m_uSoundBuffers;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAudioPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAudioPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCodecSelectionList();
	afx_msg void OnCsDownButton();
	afx_msg void OnCsUpButton();
	afx_msg void OnCsEnabledCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOPAGE_H__0E380150_021B_4DC2_BBFF_F4EF17DCC038__INCLUDED_)
