///////////////////////////////////////////////////////////////////////////////
// MyPhoneDlg.h : interface for Main Dialog class.
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

#if !defined(AFX_MYPHONEDLG_H__D73EA34B_CCAF_4862_840F_E7EA0162585D__INCLUDED_)
#define AFX_MYPHONEDLG_H__D73EA34B_CCAF_4862_840F_E7EA0162585D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPhoneEndPoint.h"	// Added by ClassView
#include "MyIndicator.h"	// For my Sound level indicator
////////////////////////////
enum STATUSMODE{S_SYSTEM, S_INMSG, S_OUTMSG};
enum VIDEOSIZE{QCIF, CIF};
//////////////////////////// some windows Messages
#define WM_FINDGATEKEEPER  (WM_USER+200)
#define WM_TRAYNOTIFY  (WM_USER + 201)
////////////////////////////
// Status Bar panes
static UINT BASED_CODE indicators[] =
{
    ID_INDICATOR_IP,	// Pane for Statistic, IP and other messages
//    ID_INDICATOR_TIME   // Not used now
};

// Define manualy some "modern" M$ structures which I do not have
#define NIF_INFO        0x00000010
#define NIIF_INFO       0x00000001
typedef struct _NOTIFYICONDATA { 
    DWORD cbSize; 
    HWND hWnd; 
    UINT uID; 
    UINT uFlags; 
    UINT uCallbackMessage; 
    HICON hIcon; 
    #if (_WIN32_IE < 0x0500)
        TCHAR szTip[64];
    #else
        TCHAR szTip[128];
    #endif
    #if (_WIN32_IE >= 0x0500)
        DWORD dwState; 
        DWORD dwStateMask; 
        TCHAR szInfo[256]; 
        union {
            UINT  uTimeout; 
            UINT  uVersion; 
        } DUMMYUNIONNAME;
        TCHAR szInfoTitle[64]; 
        DWORD dwInfoFlags; 
    #endif
    #if (_WIN32_IE >= 0x600)
        GUID guidItem;
    #endif
} MYNOTIFYICONDATA, *PMYNOTIFYICONDATA; 


/////////////////////////////////////////////////////////////////////////////
// CMyPhoneDlg dialog

class CMyPhoneDlg : public CDialog
{
// Construction
public:
	CMyPhoneDlg(CWnd* pParent = NULL);	// standard constructor
	~CMyPhoneDlg();

        CVideoDlg * vdlg;

// new maggus functions 
	BOOL OnAnswerCall(const H323Connection & connection);
	void OnConnectionEstablished(LPCTSTR remotename);
	void OnConnectionCleared(LPCTSTR remotename);
	void OutputStatus(unsigned strId, ...);
	void OutputStatusStr(LPCSTR, STATUSMODE, ...);
// sound mixer functions
// use it like SetVolume(MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT, 
//						 MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,
//						 (DWORD) 0xFF);
	bool SetVolume(DWORD inout, DWORD ComponentType, DWORD dwVol);
	DWORD GetVolume(DWORD inout, DWORD ComponentType);
	void SendUserInput(LPCTSTR strusermsg);
	void ShowStats();
	void OnUpdateIndicators();
	void MuteMicrophoneCmd();
	void MuteSpeakerCmd();
	//
	UINT statTimer;
	//
	PString m_token;
	CMyPhoneEndPoint m_endpoint;
	//
    PTimer ringSoundTimer;
    PDECLARE_NOTIFIER(PTimer, CMyPhoneDlg, OnRingSoundAgain);

    PTimer indTimer;
    PDECLARE_NOTIFIER(PTimer, CMyPhoneDlg, OnUpdateIndicators);


//    PTimer noAnswerTimer; // not using for now
//    PDECLARE_NOTIFIER(PTimer, CMyPhoneDlg, OnNoAnswerTimeout);

    PString ringSoundFile;
//    PString noAnswerForwardParty;
    PString noAnswerTime;

	PConfig adrbook;  // PhoneBook file

//  mutex for safe closing
//	Not used now because m_endpoint.ClearCall(m_token);
//	seams to be asynchronous anyway.
	HANDLE hCloseMutex;	

//  Frames are drawn now directly from CVideoOutputDevice
//	void VideoDraw(BYTE * frameStore, UINT fX, UINT fY, UINT fLen , LPCTSTR Name, UINT nScr);	//video redrow function called from CVideoDevice

// Dialog Data
	//{{AFX_DATA(CMyPhoneDlg)
	enum { IDD = IDD_MYPHONE_DIALOG };
	CButton	m_hidePnPCtrl;
	CButton	m_sndMute;
	CButton	m_micMute;
	CStatic	m_statbar;
	CButton	m_vidPan;
	CMyIndicator	m_volindicator;
	CMyIndicator	m_micindicator;
	CComboBox	m_cdestination;
	CEdit	m_usermsg;
	CSliderCtrl	m_sliderMic;
	CSliderCtrl	m_sliderVolume;
	CRichEditCtrl	m_cstatus;
	CStatic	m_caller;
	CButton	m_refuse;
	CButton	m_call;
	CString	m_destination;
	CString	m_status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPhoneDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
private:
//    CMyMainMenu* pMyMenu;

public:
	BOOL hidePnP;
	void moveControl(int ctrl, BOOL add, int iType=0);
	void changeVideoSize(VIDEOSIZE vSize);
	CString GetSimpleName(CString & fullName);
	BOOL CopyContact(CString & oldName, CString & newName);
	BOOL TrayAct;
	void AddIcon();
	void RemoveIcon();
	void ChangeIcon(CString &strMsg, int iType=0);
	CString FindContactName(const H323Connection & connection);
//	CString goodName;
	CString GetSimpleAdr(CString &);
	CString PhoneBookAddCall(int iType, CString &callName, CString & callAdr);
//	void SetMainMenu(BOOL setH=FALSE, int chSpace=0);
	void TranslateDlg();
	BOOL autoAddInAddr;
	BOOL hideSysMsg;
	BOOL autohideVideoPan;
	bool showVideoPan;
	void ShowVideoPanels(bool show = true);
	BOOL hideStat;  // can show stat
//	CStatusBar m_bar;  // Status bar
	CString m_oldStatus;  // storring old status msg
	BOOL fullScreenDefault;
	void StoreDestonation(LPCTSTR, BOOL isIncoming=FALSE);
	void LoadDestonation(void);
    virtual void OnOK();
    virtual void OnCancel();
	void OnSettings();
	void OnAppAbout();
	void OnAppHelp();
	static COLORREF colInmsg;
	static COLORREF colOutmsg;
	VIDEOSIZE curVideoSize;
	// Tracing
#if PTRACING
	BOOL Tracing;
    BOOL OpenTraceFile(PConfig & config);
	int TraceLevel;	// set -1 for no trace, or set trace detalization level (0-3).
    PTextFile * myTraceFile;
#endif

protected:
	HICON m_hIcon;
//	CFont gFont;  // font for Drawing

	// Generated message map functions
	//{{AFX_MSG(CMyPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCall();
	afx_msg void OnAnswer();
	afx_msg void OnRefuse();
	afx_msg void OnHangup();
	afx_msg void OnEditchangeDestination();
	afx_msg void OnClose();
	afx_msg void OnSelendokDestination();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBvidpan();
	afx_msg LRESULT OnLogCopy(WPARAM, LPARAM);
	afx_msg LRESULT OnLogSelectAll(WPARAM, LPARAM);
	afx_msg LRESULT OnLogClear(WPARAM, LPARAM);
	afx_msg LRESULT OnDestCopy(WPARAM, LPARAM);
	afx_msg LRESULT OnDestPaste(WPARAM, LPARAM);
	afx_msg LRESULT OnDestClear(WPARAM, LPARAM);
	afx_msg void OnBsnd();
	afx_msg void OnBmic();
	afx_msg LPARAM OnFindGatekeeper(WPARAM, LPARAM);	
	afx_msg void OnAbook();
	afx_msg void OnHidepnpcheck();
	//}}AFX_MSG
//	void OnSysCommand(UINT nID, LPARAM lParam);
	LPARAM OnTrayNotify(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

// some global functions
DWORD GetCurentIP(LPTSTR lpStringWithIP);
CString ParseBytes(int ibutes);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

class CVideoDlg : public CDialog
{
public:
        CVideoDlg();

        enum { IDD = IDD_VIDEOBOX };

	int d3d_mode;
	virtual BOOL PreTranslateMessage(MSG* pMsg);


protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual BOOL OnInitDialog();
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nID, CPoint point);
        afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
        afx_msg void OnSysKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
        afx_msg void CVideoDlg::OnClose( );
        afx_msg void CVideoDlg::OnDestroy();
        afx_msg void CVideoDlg::OnShowWindow(BOOL bShow,UINT nStatus);

        DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MYPHONEDLG_H__D73EA34B_CCAF_4862_840F_E7EA0162585D__INCLUDED_)
