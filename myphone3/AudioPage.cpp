// AudioPage.cpp : implementation file for Audio Settings Dialog class
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
#include "MyPhone.h"
#include "AudioPage.h"
//
#include "Multilang.h"		// Languages
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR OnCodecSuffix[] = _T(" (On)");
const TCHAR OffCodecSuffix[] = _T(" (Off)");
/////////////////////////////////////////////////////////////////////////////
// CAudioPage property page

IMPLEMENT_DYNCREATE(CAudioPage, CPropertyPage)

CAudioPage::CAudioPage() : CPropertyPage(CAudioPage::IDD)
{
	//{{AFX_DATA_INIT(CAudioPage)
	m_PlayDeviceCur = _T("");
	m_editBuffers = _T("");
	m_Jitter = 0;
	m_RecordDeviceCur = _T("");
	m_SilenceDetect = FALSE;
        m_AECEnable = FALSE;
	m_AGCEnable = 0;
	//}}AFX_DATA_INIT
	this->m_psp.dwFlags^=PSP_HASHELP;
	m_uSoundBuffers = 10;
}

CAudioPage::~CAudioPage()
{
}

void CAudioPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYBACK_COMBO, m_PlayDeviceBox);
	DDX_Control(pDX, IDC_RECORDING_COMBO, m_RecordDeviceBox);

	if (!pDX->m_bSaveAndValidate)
	{
		m_PlayDeviceBox.ResetContent();
		for (int i=0;i<m_PlayDeviceList.GetSize();i++)
			m_PlayDeviceBox.AddString(m_PlayDeviceList[i]);

		m_RecordDeviceBox.ResetContent();
		for (int i=0;i<m_RecordDeviceList.GetSize();i++)
			m_RecordDeviceBox.AddString(m_RecordDeviceList[i]);
	}	

	//{{AFX_DATA_MAP(CAudioPage)
	DDX_Control(pDX, IDC_SPIN_BUFFERCOUNT, m_spinBuffers);
	DDX_Control(pDX, IDC_PLAYBACK_COMBO, m_PlayDeviceBox);
	DDX_Control(pDX, IDC_RECORDING_COMBO, m_RecordDeviceBox);
	DDX_Control(pDX, IDC_CS_UP_BUTTON, m_UpBtn);
	DDX_Control(pDX, IDC_CS_ENABLED_CHECK, m_EnableBtn);
	DDX_Control(pDX, IDC_CS_DOWN_BUTTON, m_DownBtn);
	DDX_Control(pDX, IDC_CODEC_SELECTION_LIST, m_CodecListCtrl);
	DDX_CBString(pDX, IDC_PLAYBACK_COMBO, m_PlayDeviceCur);
	DDX_Text(pDX, IDC_EDIT_BUFFERS, m_editBuffers);
	DDX_Text(pDX, IDC_JITTER_EDIT, m_Jitter);
	DDX_CBString(pDX, IDC_RECORDING_COMBO, m_RecordDeviceCur);
	DDX_Check(pDX, IDC_SILENCE_DETECT_CHECK, m_SilenceDetect);
	DDX_Check(pDX, IDC_AEC_ENABLE, m_AECEnable);
	DDX_Text(pDX, IDC_AGC_EDIT, m_AGCEnable);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{
		m_CodecListCtrl.ResetContent();
		for (int i=0;i<m_CodecList.GetSize();i++)
			m_CodecListCtrl.AddString(m_CodecList[i]);
	}
	else
	{
		m_CodecList.RemoveAll();
		for (int i=0;i<m_CodecListCtrl.GetCount();i++)
		{
			CString value;
			m_CodecListCtrl.GetText(i,value);
			m_CodecList.Add(value);
		}
	}	
}


BEGIN_MESSAGE_MAP(CAudioPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAudioPage)
	ON_LBN_SELCHANGE(IDC_CODEC_SELECTION_LIST, OnSelchangeCodecSelectionList)
	ON_BN_CLICKED(IDC_CS_DOWN_BUTTON, OnCsDownButton)
	ON_BN_CLICKED(IDC_CS_UP_BUTTON, OnCsUpButton)
	ON_BN_CLICKED(IDC_CS_ENABLED_CHECK, OnCsEnabledCheck)
	ON_LBN_DBLCLK(IDC_CODEC_SELECTION_LIST, OnDoubleClickCodecSelectionList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioPage message handlers

BOOL CAudioPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_PlayDeviceBox.EnableWindow(TRUE);
	m_RecordDeviceBox.EnableWindow(TRUE);
	OnSelchangeCodecSelectionList();

	m_spinBuffers.SetRange(1, 20);
	m_spinBuffers.SetPos(m_uSoundBuffers);
	//
	TranslateDlg();
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAudioPage::OnSelchangeCodecSelectionList() 
{
	int selection = m_CodecListCtrl.GetCurSel();
	int count = m_CodecListCtrl.GetCount();

	m_UpBtn.EnableWindow(selection > 0 && selection < count);
	m_DownBtn.EnableWindow(selection >=0 && selection < count-1);
	m_EnableBtn.EnableWindow(selection >= 0 && selection < count);

	CString value;
	if (selection >= 0 && selection < count)
	{
		m_CodecListCtrl.GetText(selection,value);
		m_EnableBtn.SetCheck(value.Find(OffCodecSuffix)==-1);
	}
	else
	{
		m_EnableBtn.SetCheck(0);
	}
}

void CAudioPage::OnCsDownButton() 
{
	int selection = m_CodecListCtrl.GetCurSel();
	int count = m_CodecListCtrl.GetCount();
	if (selection<0 && selection>=count-1) return;

	CString value;
	m_CodecListCtrl.GetText(selection,value);

	m_CodecListCtrl.InsertString(selection+2,value);
	m_CodecListCtrl.DeleteString(selection);
	m_CodecListCtrl.SetCurSel(selection+1);

	OnSelchangeCodecSelectionList(); 
}

void CAudioPage::OnCsUpButton() 
{
	int selection = m_CodecListCtrl.GetCurSel();
	if (selection<=0) return;
	CString value;
	m_CodecListCtrl.GetText(selection,value);

	m_CodecListCtrl.InsertString(selection-1,value);
	m_CodecListCtrl.DeleteString(selection+1);
	m_CodecListCtrl.SetCurSel(selection-1);

	OnSelchangeCodecSelectionList(); 
}

void CAudioPage::OnCsEnabledCheck() 
{
	int selection = m_CodecListCtrl.GetCurSel();	
	if (selection<0) return;

	CString value;
	m_CodecListCtrl.GetText(selection,value);

	if (m_EnableBtn.GetCheck())
		value.Replace(OffCodecSuffix, OnCodecSuffix);
	else
		value.Replace(OnCodecSuffix, OffCodecSuffix);

	m_CodecListCtrl.DeleteString(selection);
	m_CodecListCtrl.InsertString(selection,value);
	m_CodecListCtrl.SetCurSel(selection);
}

void CAudioPage::OnDoubleClickCodecSelectionList()
{
	OnSelchangeCodecSelectionList();
	m_EnableBtn.SetCheck(1 - m_EnableBtn.GetCheck());
	OnCsEnabledCheck();
}

void CAudioPage::OnOK() 
{
	m_uSoundBuffers = m_spinBuffers.GetPos();
	CPropertyPage::OnOK();
}

void CAudioPage::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_INDEVSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_INDEVSTR));
	GetDlgItem(IDC_OUTDEVSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_OUTDEVSTR));
	GetDlgItem(IDC_BUFFSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_BUFFSTR));
	GetDlgItem(IDC_JITSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_JITSTR));
	GetDlgItem(IDC_SILENCE_DETECT_CHECK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_SLSTR));
//	GetDlgItem(IDC_AEC_ENABLE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_SLSTR));
	GetDlgItem(IDC_CODECSSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CODECSSTR));	
	GetDlgItem(IDC_CS_UP_BUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CSUPSTR));	
	GetDlgItem(IDC_CS_ENABLED_CHECK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CSENBLSTR));	
	GetDlgItem(IDC_CS_DOWN_BUTTON)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CSDOWNSTR));
//	SetWindowText((LPCTSTR)LoadStringLang(IDS_AUDIOSTR));	
}
