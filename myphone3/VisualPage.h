///////////////////////////////////////////////////////////////////////////////
// VisualPage.h : interface for Visual/Video Settings Dialog class.
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

#if !defined(AFX_VISUALPAGE_H__4837478C_7F2A_49FD_9FBD_EEB659D0CA0D__INCLUDED_)
#define AFX_VISUALPAGE_H__4837478C_7F2A_49FD_9FBD_EEB659D0CA0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisualPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisualPage dialog

class CVisualPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CVisualPage)

// Construction
public:
	CVisualPage();
	~CVisualPage();
	void TranslateDlg();

// Dialog Data
	//{{AFX_DATA(CVisualPage)
	enum { IDD = IDD_VIDEOPAGE };
	CSpinButtonCtrl	m_videoInBPSspin;
        CSpinButtonCtrl m_videoOutBPSspin;
	CSpinButtonCtrl	m_videoFPSspin;
	CSpinButtonCtrl	m_videoQTYspin;
	BOOL	m_sendVideo;
	BOOL	m_receiveVideo;
	CButton		m_UpBtn;
	CButton		m_DownBtn;
	CButton		m_EnableBtn;
        CButton		m_CamSetBtn;
	CListBox	m_CodecListCtrl;
	CComboBox	m_RecordDeviceBox;
	CString	m_RecordDeviceCur;
	BOOL	m_localVideo;
	BOOL	m_LVflip;
	BOOL	m_RVflip;
	CString	m_vFPS;
	CString	m_vINBPS;
        CString m_vOUTBPS;
	CString	m_vQTY;
	int		m_RecDevSrc;
	int		m_videoInSize;
        int             m_videoOutSize;
	BOOL	m_localFlip;
	//}}AFX_DATA

	CStringArray m_RecordDeviceList;
	CStringArray m_CodecList;
	unsigned	m_videoInMaxBPS;
        unsigned        m_videoOutMaxBPS;
	unsigned	m_videoFPS;
	unsigned	m_videoQTY;
	BOOL m_showVideoSize;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CVisualPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CVisualPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCodecSelectionList();
	afx_msg void OnCsUpButton();
	afx_msg void OnCsDownButton();
        afx_msg void OnCamSetupButton();
	afx_msg void OnCsEnabledCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedBpsstatic2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALPAGE_H__4837478C_7F2A_49FD_9FBD_EEB659D0CA0D__INCLUDED_)
