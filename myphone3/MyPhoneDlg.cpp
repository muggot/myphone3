///////////////////////////////////////////////////////////////////////////////
// MyPhoneDlg.cpp : Main Dialog implementation file
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
//#include "MyMainMenu.h"
#include "MyPhoneDlg.h"

// settings pages
#include "GeneralPage.h"
#include "NetworkPage.h"
#include "guiPage.h"
#include "GKPage.h"
#include "AudioPage.h"
#include "VisualPage.h"
//
#include "MyPropertySheet.h"
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
//
#define MAXVOLUME ((WORD)0xFFFF)
//
COLORREF	CMyPhoneDlg::colInmsg;
COLORREF	CMyPhoneDlg::colOutmsg;
//
extern CMyPhoneApp theApp;
extern int curLang;
/////////////////////////////////////////////////////////////////////////////
// Configuration Keys
const char AddressBookSection[] = "AddressBook";
const char AddressBookRescentKey[] = "RescentEntries";
const char RingSoundFileConfigKey[] = "RingSoundFile";
const char IpTosConfigKey[] = "IpTOS";
const char BandwidthTypeConfigKey[] = "BandwidthType";
const char BandwidthConfigKey[] = "Bandwidth";
const char ListenerInterfaceConfigKey[] = "ListenerInterface";
const char RouterConfigKey[] = "NATRouterAddress";
//const char TCPPortBaseConfigKey[] = "TCPPortBase";
//const char TCPPortMaxConfigKey[] = "TCPPortMax";
//const char UDPPortBaseConfigKey[] = "UDPPortBase";
//const char UDPPortMaxConfigKey[] = "UDPPortMax";
const char RTPPortBaseConfigKey[] = "RTPPortBase";
const char RTPPortMaxConfigKey[] = "RTPPortMax";
const char NoTunnelingConfigKey[] = "NoTunneling";
const char DtmfAsStringConfigKey[] = "DtmfAsString";
const char AutoAnswerConfigKey[] = "AutoAnswer";
const char UsernameConfigKey[] = "Username";
//const char AliasConfigKey[] = "Alias %d";
const char AliasConfigKey[] = "UserAliases";
const char UseGatekeeperConfigKey[] = "UseGatekeeper";
const char GatekeeperPassConfigKey[] = "GatekeeperPassword";
const char RequireGatekeeperConfigKey[] = "RequireGatekeeper";
const char GatekeeperHostConfigKey[] = "GatekeeperHost";
const char GatewayHostConfigKey[] = "GatewayHost";
const char DiscoverGatekeeperConfigKey[] = "DiscoverGatekeeper";
const char DiscoverGatewayConfigKey[] = "DiscoverGateway";
const char SoundPlayConfigKey[] = "SoundPlayDevice";
const char SoundRecordConfigKey[] = "SoundRecordDevice";
const char SilenceDetectConfigKey[] = "SilenceDetect";
const char AECEnableConfigKey[] = "AECEnable";
const char AGCEnableConfigKey[] = "AGCEnable";
const char JitterConfigKey[] = "Jitter";
const char CodecsConfigSection[] = "Codecs";
const char VideoCodecsConfigSection[] = "VideoCodecs";
const char NoFastStartConfigKey[] = "FastStart";
const char StrictSingleLineConfigKey[] = "SingleLine";
const char BufferCountConfigKey[] = "BufferCount";
const char AutoReceiveVideoConfigKey[] = "AutoReceiveVideo";
const char AutoTransmitVideoConfigKey[] = "AutoTransmitVideo";
const char VideoDeviceConfigKey[] = "VideoDevice";
const char VideoInSizeConfigKey[] = "VideoInSize";
const char VideoOutSizeConfigKey[] = "VideoOutSize";
const char VideoSourceConfigKey[] = "VideoSource";
const char VideoFormatConfigKey[] = "VideoFormat";
const char VideoQualityConfigKey[] = "VideoQuality";
const char VideoLocalConfigKey[] = "VideoLocal";
const char VideoFlipLocalConfigKey[] = "VideoFlipLocal";
const char VideoFPSKey[] = "VideoFramesPerSecond";
const char VideoInMaxbandWidthKey[] = "VideoInMaxbandwidth";
const char VideoOutMaxbandWidthKey[] = "VideoOutMaxbandwidth";
const char VideoPacketSizeConfigKey[] = "VideoPacketSize";
//const char VideoFlipRemoteConfigKey[] = "VideoFlipRemote";
const char VideoInVFlipConfigKey[] = "VideoInVFlip";
const char VideoInHFlipConfigKey[] = "VideoInHFlip";
const char VideoOutVFlipConfigKey[] = "VideoOutVFlip";
const char VideoOutHFlipConfigKey[] = "VideoOutHFlip";
const char OutputVolumeConfigKey[] = "OutputVolume";
const char ColorInMessageConfigKey[] = "ColorInMessage";
const char ColorOutMessageConfigKey[] = "ColorOutMessage";
const char ColorSignalIndicatorConfigKey[] = "ColorActInd";
const char ColorSilentIndicatorConfigKey[] = "ColorSlntInd";
const char AutoVideoHideConfigKey[] = "AutoVidPanHide";
const char HideStatConfigKey[] = "HideStatistic";
const char FullScreenDefaultConfigKey[] = "FullScreenDefault";
const char SysLogMsgHideConfigKey[] = "HideSysLogMessages";
const char AutoAddCallersConfigKey[] = "AutoAddCallerAddr";
const char UILangugeConfigKey[] = "UILanguge";

#if PTRACING
const char TraceFileConfigKey[] = "TraceFile";
const char TraceLevelConfigKey[] = "TraceLevel";
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_sdesc;
	CButton	m_BWeb;
	CStatic	m_scprt;
	CStatic	m_stitle;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBhomepage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_DESK, m_sdesc);
	DDX_Control(pDX, IDC_BHOMEPAGE, m_BWeb);
	DDX_Control(pDX, IDC_SCPRT, m_scprt);
	DDX_Control(pDX, IDC_STITLE, m_stitle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
ON_BN_CLICKED(IDC_BHOMEPAGE, OnBhomepage)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strTitle;
	char strProgVersion[16];
#ifdef MF_RELEASE
	sprintf(strProgVersion, "%d.%d", progMajorVersion, progMinorVersion);
#else
	sprintf(strProgVersion, "%d.%db%d%s", progMajorVersion, progMinorVersion, progBuildVersion, strlen(strProgPostfix)>0?strProgPostfix:"");
#endif
	
	strTitle.Format((LPCTSTR)LoadStringLang(IDS_STRPROGNAMEVER), strProgName, strProgVersion);
	m_stitle.SetWindowText((LPCTSTR)strTitle);
	m_sdesc.SetWindowText(LoadStringLang(IDS_STRPROGDESC)+_T("\r\n")+CString(strProgRespects)+_T("\r\n")+LoadStringLang(IDS_PROGTRANSSTR)+_T("\r\n\r\nH323plus\t2008"));
	m_BWeb.SetWindowText(strProgWeb);
	m_scprt.SetWindowText(strProgAuthor);
	SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOUTSTR));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnBhomepage() 
{
	ShellExecute(NULL, "open", strProgWeb, NULL, NULL, SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneDlg dialog

CMyPhoneDlg::CMyPhoneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMyPhoneDlg::IDD, pParent)
{
#if PTRACING
    myTraceFile = NULL;
    TraceLevel = -1;
	CString cmdLine(theApp.m_lpCmdLine);
	cmdLine.MakeLower();
	if(cmdLine.Find(_T("-t"))>=0)     TraceLevel = 1;
	if(cmdLine.Find(_T("-tt"))>=0)     TraceLevel = 2;
	if(cmdLine.Find(_T("-ttt"))>=0)     TraceLevel = 3;
	if(cmdLine.Find(_T("-tttt"))>=0)     TraceLevel = 4;
	if(cmdLine.Find(_T("-ttttt"))>=0)     TraceLevel = 5;
	if(cmdLine.Find(_T("-tttttt"))>=0)     TraceLevel = 6;
	if((!cmdLine.IsEmpty()) && (TraceLevel<0)) 
	{
	AfxMessageBox("MyPhone command line options:\n\
				  -t      :  Enable Trace level 1\n\
				  -tt     :  Enable Trace level 2\n\
				  -ttt    :  Enable Trace level 3\n\
				  -tttt   :  Enable Trace level 4\n\
				  -ttttt  :  Enable Trace level 5\n\
				  -tttttt :  Enable Trace level 6\n\
				 )", 
				 MB_ICONINFORMATION);
	}
	PConfig& config = m_endpoint.config; 
	if(TraceLevel>=0)
		Tracing = OpenTraceFile(config);
#endif
	
	//{{AFX_DATA_INIT(CMyPhoneDlg)
	m_destination = _T("");
	m_status = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_OHICON);
	// for RichEdit to work
	AfxInitRichEdit();
	//
	hCloseMutex = CreateMutex(NULL, FALSE, "CanCloseMutex");
	// making font for drawing
	/*	CFont gFont;
	VERIFY(gFont.CreateFont(
	12,                         // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	DEFAULT_CHARSET,              // nCharSet
	OUT_DEVICE_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	PROOF_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	"Verdana"));                 // lpszFacename
	*/
}

CMyPhoneDlg::~CMyPhoneDlg()
{
	//	gFont.DeleteObject(); 
	// free menu 
	/*	if (pMyMenu)
	{
	pMyMenu->Detach();
	delete pMyMenu;
	pMyMenu = 0;
	}
	*/	
    if(WaitForSingleObject( hCloseMutex, 3000L)!=WAIT_OBJECT_0)
	{
		AfxMessageBox("Sinchronization failed!");
	}
	//	if(statTimer) KillTimer(statTimer);
}


void CMyPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPhoneDlg)
	DDX_Control(pDX, IDC_HIDEPNPCHECK, m_hidePnPCtrl);
	DDX_Control(pDX, IDC_BSND, m_sndMute);
	DDX_Control(pDX, IDC_BMIC, m_micMute);
	DDX_Control(pDX, IDC_SMYSTAT, m_statbar);
	DDX_Control(pDX, IDC_BTEST, m_vidPan);
	DDX_Control(pDX, IDC_VOLIND, m_volindicator);
	DDX_Control(pDX, IDC_MICIND, m_micindicator);
	DDX_Control(pDX, IDC_DESTINATION, m_cdestination);
	DDX_Control(pDX, IDC_USERMSG, m_usermsg);
	DDX_Control(pDX, IDC_SLIDER_MIC, m_sliderMic);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sliderVolume);
	DDX_Control(pDX, IDC_STATUS, m_cstatus);
	DDX_Control(pDX, IDC_CALLER, m_caller);
	DDX_Control(pDX, IDC_REFUSE, m_refuse);
	DDX_Control(pDX, IDC_CALL, m_call);
	DDX_Text(pDX, IDC_DESTINATION, m_destination);
	DDX_Text(pDX, IDC_STATUS, m_status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPhoneDlg, CDialog)
//{{AFX_MSG_MAP(CMyPhoneDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_CALL, OnCall)
ON_BN_CLICKED(IDC_ANSWER, OnAnswer)
ON_BN_CLICKED(IDC_REFUSE, OnRefuse)
ON_BN_CLICKED(IDC_HANGUP, OnHangup)
ON_CBN_EDITCHANGE(IDC_DESTINATION, OnEditchangeDestination)
ON_CBN_SELENDOK(IDC_DESTINATION, OnSelendokDestination)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BVIDPAN, OnBvidpan)
ON_MESSAGE(ID_LOGMENU_COPY, OnLogCopy)
ON_MESSAGE(ID_LOGMENU_SELECTALL, OnLogSelectAll) 
ON_MESSAGE(ID_LOGMENU_CLEARLOG_OK, OnLogClear)
ON_MESSAGE(ID_DESTMENU_COPY, OnDestCopy)
ON_MESSAGE(ID_DESTMENU_PASTE, OnDestPaste)
ON_MESSAGE(ID_DESTMENU_CLEARADDRESSES_ALL, OnDestClear)
ON_BN_CLICKED(IDC_BSND, OnBsnd)
ON_BN_CLICKED(IDC_BMIC, OnBmic)
ON_BN_CLICKED(IDC_ABOOK, OnAbook)
	ON_BN_CLICKED(IDC_HIDEPNPCHECK, OnHidepnpcheck)
	//}}AFX_MSG_MAP
ON_MESSAGE(WM_FINDGATEKEEPER, OnFindGatekeeper)
ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotify)
ON_WM_SYSCOMMAND()  
ON_COMMAND(ID_MSETTINGS, OnSettings) 
ON_COMMAND(ID_MABOUT, OnAppAbout)
ON_COMMAND(ID_MHELP, OnAppHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneDlg message handlers

// Dummy Functions to prevent closing
void CMyPhoneDlg::OnOK() 
{
}

void CMyPhoneDlg::OnCancel() 
{
}

/////////////////////////////////////////////////////////////////////////////
BOOL CMyPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	AddIcon();
	//
	PTRACE(4, "MyPhone\t###OnInitDialog###\tMaking Sysytem Menu");
	
	PConfig& config = m_endpoint.config; 
	curLang = GetLangFromTable(config.GetInteger(UILangugeConfigKey, -1));
	// RichEdit configuration
	m_cstatus.HideSelection(FALSE, TRUE);
	m_cstatus.SetBackgroundColor( FALSE , ::GetSysColor(COLOR_3DFACE));
	m_cstatus.SetEventMask(ENM_MOUSEEVENTS); // redirect mouse events to main window
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	// Adding "About" to system menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu = LoadStringLang(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Changing size of Main Menu
	//	pMyMenu = new CMyMainMenu;
	//	if (pMyMenu != NULL)
	//	{
	//		pMyMenu->LoadMenu(IDR_MMAIN);
	//		pMyMenu->ModifyMenu(1, MF_OWNERDRAW | MF_BYPOSITION );
	//		HMENU hMenu = ::GetMenu(GetSafeHwnd());
	//		pMyMenu->Attach(hMenu);
	//		CMenu *oldMenu = GetMenu();
	//		SetMenu(NULL);
	//		if(oldMenu)
	//			oldMenu->DestroyMenu();
	//		SetMenu(pMyMenu);
	//		SetMainMenu(TRUE);
	//	}
	
	//	CMenu* pMainMenu = GetMenu();
	//	if (pMainMenu != NULL)
	//	{
	//		pMainMenu->ModifyMenu(1, MF_OWNERDRAW | MF_BYPOSITION );
	//		DrawMenuBar();
	//	}
	
	//Default messages colors
	colInmsg = RGB(0,0,153);
	colOutmsg = RGB(0,153,0);
	
	/////// Creating StatusBar
	//	m_bar.Create(this); //We create the status bar
	//	m_bar.SetIndicators(indicators, 1 /*2*/); //Set the number of panes 
	//	CRect rect;
	//	GetClientRect(&rect);
	//Size the two panes
	//	RECT rWnd;
	//	GetClientRect(&rWnd);
	//	m_bar.SetPaneInfo(0,ID_INDICATOR_IP, SBPS_NORMAL,rect.Width() /*-(rWnd.right>0?rWnd.right/2:250)*/);      
	//	m_bar.SetPaneInfo(1,ID_INDICATOR_TIME,SBPS_STRETCH ,0);
	//	m_bar.SetPaneInfo(0,ID_INDICATOR_IP,SBPS_STRETCH ,0);
	//	m_bar.SetPaneInfo(0,ID_INDICATOR_IP,SBPS_NORMAL, rect.Width()-50);
	//This is where we actually draw it on the screen
	//	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,
	//		ID_INDICATOR_IP /*ID_INDICATOR_TIME*/);
	
	PTRACE(4, "MyPhone\t###OnInitDialog###\tGetting local IP");
	// getting IP
    char s[64];
	if(GetCurentIP(s))
	{
		CString sIP;
		sIP.Format((LPCTSTR) LoadStringLang(IDS_UIPSTR), s);
		if(sIP.Find("127.0.0.1", 9)>0 || sIP.Find("3.0", 9)>0)
			sIP+=LoadStringLang(IDS_NONETSTR);//CString(_T(" - WARNING! No network detected"));
		else
			if((sIP.Find("192.168.", 9)>0) || (sIP.Find("10.", 9)>0) || (sIP.Find("127.", 9)>0))
				sIP+=LoadStringLang(IDS_PROXINETSTR);
			m_statbar.SetWindowText(sIP.GetBuffer(0));
			//m_bar.SetPaneText(0,sIP.GetBuffer(0));
			m_oldStatus=sIP;
	}
	// Setting Main window title 
	CString strTitle;
	char strProgVersion[16];
#ifdef MF_RELEASE
	sprintf(strProgVersion, "%d.%d", progMajorVersion, progMinorVersion);
#else
	sprintf(strProgVersion, "%d.%db%d%s", progMajorVersion, progMinorVersion, progBuildVersion, strlen(strProgPostfix)>0?strProgPostfix:"");
#endif
	strTitle.Format("%s %s", strProgName, strProgVersion);
	this->SetWindowText(strTitle);
	//	m_destination="localhost";
	m_usermsg.SetWindowText((LPCTSTR) LoadStringLang(IDS_UMSGSTR));
	// loading last destinatins
	PTRACE(4, "MyPhone\t###OnInitDialog###\tLoading old destinations");
	LoadDestonation();
	PTRACE(4, "MyPhone\t###OnInitDialog###\tSetting window timer");
	// Main timer
	statTimer = SetTimer(100, 3000, NULL);
	hideStat = config.GetBoolean(HideStatConfigKey, FALSE); // Can show statistic info
	fullScreenDefault = config.GetBoolean(FullScreenDefaultConfigKey, FALSE);
	indTimer.SetNotifier(PCREATE_NOTIFIER(OnUpdateIndicators));
	
	// setting icons
	m_micMute.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MICICON)));
	m_sndMute.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SNDICON)));	
	m_call.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CALLICON)));	
	//	m_answer.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CALLANSWICON)));	
	m_refuse.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CALLREFICON)));	
	//	m_hangup.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CALLHANGICON)));	
	
	PTRACE(4, "MyPhone\t###OnInitDialog###\tSetting Volume");
	//out volume and buttons
	DWORD dwVolume = 0L;
	m_sliderVolume.SetRange(0, MAXVOLUME);
	//	VERIFY(MMSYSERR_NOERROR == waveOutGetVolume(0, &dwVolume));
	dwVolume=GetVolume(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,
		MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT);
	if(dwVolume>=0)
    {
		m_sliderVolume.SetPos( (MAXVOLUME & dwVolume));
		m_sliderVolume.EnableWindow(TRUE);
    }
	else
		m_sliderVolume.EnableWindow(FALSE);
	// in volume
	m_sliderMic.SetRange(0, MAXVOLUME);
	dwVolume=GetVolume(MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
		MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE);
	if(dwVolume>=0)
    {
		m_sliderMic.SetPos( (MAXVOLUME & dwVolume));
		m_sliderMic.EnableWindow(TRUE);
    }
	else
		m_sliderMic.EnableWindow(FALSE);
	
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Reaad Ring sound settings from Config
	ringSoundFile = config.GetString(RingSoundFileConfigKey, _T(""));
	ringSoundTimer.SetNotifier(PCREATE_NOTIFIER(OnRingSoundAgain));
	// Read colors
	CMyPhoneDlg::colInmsg = config.GetInteger(ColorInMessageConfigKey, CMyPhoneDlg::colInmsg);
	CMyPhoneDlg::colOutmsg = config.GetInteger(ColorOutMessageConfigKey, CMyPhoneDlg::colOutmsg);
	CMyIndicator::clrFg = config.GetInteger(ColorSignalIndicatorConfigKey, CMyIndicator::clrFg);
	CMyIndicator::clrSFg = 	config.GetInteger(ColorSilentIndicatorConfigKey, CMyIndicator::clrSFg);
	// Video panel settings
	autohideVideoPan = config.GetBoolean(AutoVideoHideConfigKey, TRUE);
	showVideoPan = TRUE;
	m_vidPan.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LARRICON)));
	if(autohideVideoPan)
		ShowVideoPanels(FALSE);  // hide Video panel in the begining
	hidePnP = FALSE;
	m_hidePnPCtrl.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PNPSHOW)));
	m_hidePnPCtrl.ShowWindow(SW_HIDE);
	m_endpoint.localVideo = config.GetBoolean(VideoLocalConfigKey, TRUE);
//	curVideoSize = QCIF;
//	int videoInSize = config.GetInteger(VideoInSizeConfigKey, 2);
//	if(videoInSize == 2)
//		changeVideoSize(QCIF);
//	else if(videoInSize == 3)
//		changeVideoSize(CIF);
	// Show/Hide system log messages
	hideSysMsg = config.GetBoolean(SysLogMsgHideConfigKey, FALSE);
	// Add incoming callers automaticly to destinations
	autoAddInAddr = config.GetBoolean(AutoAddCallersConfigKey, TRUE);
	// setting languge for window/controls 
	PTRACE(4, "MyPhone\t###OnInitDialog###\tSetting UI language");
	TranslateDlg();

	CVideoDlg *vdlg = NULL;

	vdlg=new CVideoDlg();
	vdlg->Create(CVideoDlg::IDD,NULL);
//	vdlg->ShowWindow(SW_SHOW);
        if(fullScreenDefault) vdlg->d3d_mode = 1;

	// Initialize OpenH323 Endpoint
	PTRACE(4, "MyPhone\t###OnInitDialog###\tCall Endpoint Initialize");
	m_endpoint.Initialise(this,vdlg);
	// Find Gatekeeper if needed
	PTRACE(4, "MyPhone\t###OnInitDialog###\tPosting FINDGATEKEEPER event");
	PostMessage(WM_FINDGATEKEEPER);
	
#if PTRACING
	if(TraceLevel>=0)
	{
		if(Tracing)
		{
			CString strMsg;
			strMsg.Format("%d", TraceLevel);
			OutputStatusStr("Trace started.\n   Level: %1\n   Output: %2", 
				S_SYSTEM, (LPCTSTR) strMsg, (LPCTSTR) (myTraceFile->GetName()));
		}
		else
			OutputStatusStr("ERROR! Trace failed.", S_SYSTEM); 
	}
#endif
	
	// Init PhoneBook
	adrbook = PConfig(PFilePath(".\\mphnbook.mph"), "-empty-");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyPhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_CLOSE)  // Closing from pressing [X]
	{
		OnClose();
		CDialog::EndDialog(1);
	}
	else if(nID == SC_MINIMIZE)
	{
		CDialog::OnSysCommand(nID, lParam);
		ShowWindow(SW_HIDE);
	}
	else if(nID == SC_MAXIMIZE)
	{
		ShowWindow(SW_SHOW);
		CDialog::OnSysCommand(nID, lParam);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyPhoneDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyPhoneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// writing status
void CMyPhoneDlg::OutputStatus(unsigned strId, ...)
{
	//  if (currentCallMode == ApplicationShuttingDown)
	//    return;
	if(hideSysMsg)  return;
	
	CString test;
	test=LoadStringLang(strId);
	
	LPVOID lpMsgBuf;
	va_list args;
	va_start(args, strId);
	
	DWORD len=FormatMessage( 
        FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
        test.GetBuffer(0),   // NULL 
        0, 
        0,
        (LPTSTR) &lpMsgBuf, 
        0, 
        &args); 
	
	if(len && strlen((LPCTSTR)lpMsgBuf))
	{
		CString strNew, strOld;
		m_cstatus.GetWindowText(strOld);
		m_cstatus.SetSel( strOld.GetLength(), strOld.GetLength());	
		strNew=CString((LPCTSTR)lpMsgBuf)+CString("\r\n");
		m_cstatus.ReplaceSel((LPCTSTR) strNew.GetBuffer(0));
		m_cstatus.SetSel( strNew.GetLength()+strOld.GetLength(), strNew.GetLength()+strOld.GetLength());	
		
	}
	LocalFree( lpMsgBuf ); 
	va_end (args); 
}

void CMyPhoneDlg::OutputStatusStr(LPCSTR strF, STATUSMODE mode, ...)
{
	// log output from strrings formating
	if((hideSysMsg) && (mode==S_SYSTEM))
		return;
	
	LPVOID lpMsgBuf;
	va_list args;
	va_start(args, mode);
	
	DWORD len=FormatMessage( 
        FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
        strF,   // NULL 
        0, 
        0,   /// curLanguageId
        (LPTSTR) &lpMsgBuf, 
        0, 
        &args); 
	
	if(len && strlen((LPCTSTR)lpMsgBuf))
	{
		// formatting
		CHARFORMAT cf, oldcf;
		m_cstatus.GetDefaultCharFormat(cf);
		m_cstatus.GetDefaultCharFormat(oldcf);
		if(mode==S_OUTMSG) // Outgoing chat message
		{
			cf.dwMask = CFM_COLOR|CFM_BOLD;
			cf.crTextColor = colOutmsg;
			cf.dwEffects = CFE_DISABLED;
		}
		if(mode==S_INMSG) // Incoming chat message
		{
			cf.dwMask = CFM_COLOR|CFM_BOLD;
			cf.crTextColor = colInmsg;
			cf.dwEffects = CFE_DISABLED;
		}
		
		//      long nStartChar, nEndChar;
		//      m_cstatus.GetSel(nStartChar, nEndChar);
		CString strNew, strOld;
		m_cstatus.GetWindowText(strOld);
		m_cstatus.SetSel( strOld.GetLength(), strOld.GetLength());	
		m_cstatus.SetSelectionCharFormat(cf);
		if(TRUE) // Will change later
			ChangeIcon(CString((LPCTSTR)lpMsgBuf), 2);
		strNew=CString((LPCTSTR)lpMsgBuf)+CString("\r\n");
		m_cstatus.ReplaceSel((LPCTSTR) strNew.GetBuffer(0));
		m_cstatus.SetSel( strNew.GetLength()+strOld.GetLength(), strNew.GetLength()+strOld.GetLength());	
		m_cstatus.SetSelectionCharFormat(oldcf);
	}
	LocalFree( lpMsgBuf ); 
	va_end (args); 
}

//CVideoDlg *vdlg = NULL;

// Call related buttons actions
void CMyPhoneDlg::OnCall() 
{
//if(vdlg==NULL)
//{
// vdlg=new CVideoDlg();
// vdlg->Create(CVideoDlg::IDD,NULL);
// vdlg->ShowWindow(SW_SHOW);
//}
	if(!m_endpoint.HasConnection(m_token))
	{
		UpdateData();
		m_call.EnableWindow(FALSE);
		m_refuse.EnableWindow();
		GetDlgItem(IDC_REFUSE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_HANGUPBSTR));	
		//
		CString curDest = m_destination;
		curDest.TrimLeft();
		int iPos=-1;
		if((iPos=curDest.Find(" "))>0)
			curDest = curDest.Left(iPos);
		m_endpoint.MakeCall((const char *)curDest, m_token);
		// Maybe this should be moved to OnConnectionEsteblished
		PhoneBookAddCall(1, curDest, curDest);
	}
	else
	{
		ringSoundTimer.Stop();
		m_caller.SetWindowText("");
		//		m_answer.EnableWindow(FALSE);
		m_refuse.EnableWindow(FALSE);
		//		m_hangup.EnableWindow(FALSE);
		m_call.EnableWindow();
		GetDlgItem(IDC_CALL)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CALLBSTR));
		
		H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
		if (connection == NULL)
			m_call.EnableWindow();
		else 
		{
			connection->AnsweringCall(H323Connection::AnswerCallNow);
			connection->Unlock();
		}
	}
}

void CMyPhoneDlg::OnAnswer() 
{
	ringSoundTimer.Stop();
	m_caller.SetWindowText("");
	//  m_answer.EnableWindow(FALSE);
	m_refuse.EnableWindow(FALSE);
	//  m_hangup.EnableWindow(FALSE);
	m_call.EnableWindow();
	
	H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
	if (connection == NULL)
		m_call.EnableWindow();
	else {
		connection->AnsweringCall(H323Connection::AnswerCallNow);
		connection->Unlock();
	}
}

void CMyPhoneDlg::OnRefuse() 
{
	if(!m_endpoint.HasConnection(m_token))
	{
		m_caller.SetWindowText("");
		//		m_answer.EnableWindow(FALSE);
		m_refuse.EnableWindow(FALSE);
		//		m_hangup.EnableWindow(FALSE);
		
		H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
		if (connection == NULL)
			m_call.EnableWindow();
		else {
			connection->AnsweringCall(H323Connection::AnswerCallDenied);
			connection->Unlock();
		}
	}
	else
	{
		m_endpoint.ClearCall(m_token);
		//		m_hangup.EnableWindow(FALSE);
		//		m_call.EnableWindow();
	}
}

void CMyPhoneDlg::OnHangup() 
{
	m_endpoint.ClearCall(m_token);
	//  m_hangup.EnableWindow(FALSE);
	if(m_endpoint.GetAllConnections().GetSize()<2)
		m_call.EnableWindow();
}

void CMyPhoneDlg::OnConnectionEstablished(LPCTSTR remotename)
{
	// UI change when call starts
	// called from End Point class
	m_call.EnableWindow(FALSE);
	m_refuse.EnableWindow(TRUE);
	GetDlgItem(IDC_REFUSE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_HANGUPBSTR));	
	m_caller.SetWindowText(LoadStringLang(IDS_TALKINGSTR) + CString(remotename));
	OutputStatus(IDS_TALKING, remotename);
	// message window ON
	m_usermsg.SetWindowText(_T(""));
	m_usermsg.EnableWindow(TRUE);
	//  SetCallMode(Active323CallMode);
	indTimer.RunContinuous(200);
	//  if(autohideVideoPan && showVideoPan )
	//     ShowVideoPanels(FALSE);  // hide Video panel if it's visible
	m_micMute.SetCheck(BST_UNCHECKED);
	m_micMute.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MICICON)));
}

void CMyPhoneDlg::OnConnectionCleared(LPCTSTR remotename)
{
    if(m_endpoint.GetAllConnections().GetSize()<2){
	// UI change after call ended
	// called from End Point class
	m_volindicator.Reset();
	m_micindicator.Reset();
	indTimer.Stop();
	ringSoundTimer.Stop();
	//
	if(autohideVideoPan && showVideoPan)	
		ShowVideoPanels(FALSE);  // hide Video panel if it's visible
	//  m_answer.EnableWindow(FALSE);
	m_refuse.EnableWindow(FALSE);
	//  m_hangup.EnableWindow(FALSE);
	GetDlgItem(IDC_REFUSE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_REFUSEBSTR));
	m_call.EnableWindow();
	GetDlgItem(IDC_CALL)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CALLBSTR));
	m_caller.SetWindowText("");
	OutputStatus(IDS_STOPCONNECT, remotename);
	// message window OFF
	m_usermsg.SetWindowText((LPCTSTR)LoadStringLang(IDS_UMSGSTR));
	m_usermsg.EnableWindow(FALSE);
	GetDlgItem(IDC_VIDEOBITMAP)->RedrawWindow();
	GetDlgItem(IDC_VIDEOBITMAP1)->RedrawWindow();
	GetDlgItem(IDC_SLVIDEO)->SetWindowText(_T(""));
	GetDlgItem(IDC_SRVIDEO)->SetWindowText(_T(""));
	m_micMute.SetCheck(BST_UNCHECKED);
	m_micMute.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MICICON)));
   }
}

void CMyPhoneDlg::OnEditchangeDestination() 
{
	UpdateData();
	m_call.EnableWindow((!m_destination.IsEmpty()) && (!m_endpoint.HasConnection(m_token)));	
}

void CMyPhoneDlg::OnSelendokDestination() 
{
	CString strSel;
	m_cdestination.GetLBText(m_cdestination.GetCurSel(), strSel);
	m_call.EnableWindow(!strSel.IsEmpty() && (!m_endpoint.HasConnection(m_token)));
}

////////////////////////////////////////////////////////////////
// Menu functions
void CMyPhoneDlg::OnSettings()
{
	PConfig& config = m_endpoint.config; 
	CMyPropertySheet Dlg((LPCTSTR) LoadStringLang(IDS_SETTINGSSTR),this,0);
	int i=0;
	
	CGeneralPage genPage;
	Dlg.AddPage(&genPage);
	
	genPage.m_UserName = (const char*)(m_endpoint.GetLocalUserName());
	
	genPage.m_RingSoundFile = (const char*) ringSoundFile;
	
	genPage.m_AutoAnswer = m_endpoint.m_fAutoAnswer;
	genPage.m_DtmfAsString = m_endpoint.m_fDtmfAsString;
	genPage.m_NoTunneling = m_endpoint.DisableH245Tunnelling();
	genPage.m_disableFastStart = m_endpoint.m_fNoFastStart;
	genPage.m_singleLine = m_endpoint.m_fStrictSingleLine;
	
	CNetworkPage netPage;
	Dlg.AddPage(&netPage);
	
	netPage.m_BandwidthType = config.GetInteger(BandwidthTypeConfigKey, 5);
	netPage.m_Bandwidth = m_endpoint.GetInitialBandwidth()/20;
	netPage.m_IP_Type = m_endpoint.GetRtpIpTypeofService();
	netPage.m_rtpbase = config.GetInteger(RTPPortBaseConfigKey, m_endpoint.GetRtpIpPortBase());
	netPage.m_rtpmax = config.GetInteger(RTPPortMaxConfigKey, m_endpoint.GetRtpIpPortBase());
	netPage.m_natip = 	(config.HasKey(RouterConfigKey))?
		(LPCTSTR)config.GetString(RouterConfigKey, m_endpoint.m_router.AsString()):
	_T("");
	PString lifaces = config.GetString(ListenerInterfaceConfigKey, "*");
	lifaces = lifaces.IsEmpty()?"*":lifaces;
	netPage.m_linterface = (LPCTSTR)lifaces;
	
	CGKPage gatePage;
	Dlg.AddPage(&gatePage);
	
	gatePage.m_gkuse = config.GetBoolean(UseGatekeeperConfigKey, FALSE);
	gatePage.m_gkhost = (const char*)(config.GetString(GatekeeperHostConfigKey));
	gatePage.m_gkrec = config.GetBoolean(RequireGatekeeperConfigKey, FALSE);
	//	gatePage.m_GatewayHost = (const char*)(config.GetString(GatewayHostConfigKey));
	gatePage.m_gkpass = (const char*)(config.GetString(GatekeeperPassConfigKey));
	int defGKdescover = 0;
	if(!gatePage.m_gkhost.IsEmpty()) defGKdescover=1;
	if(!gatePage.m_gkid.IsEmpty()) defGKdescover=2;
	
	gatePage.m_gkdiscover = config.GetInteger(DiscoverGatekeeperConfigKey, defGKdescover);
	//	gatePage.m_DiscoverGateway = config.GetBoolean(DiscoverGatewayConfigKey, gatePage.m_GatewayHost.IsEmpty());
	
	const PStringList aliases = m_endpoint.GetAliasNames();
	for (i = 1; i < aliases.GetSize(); i++)
		gatePage.m_AliasList.Add(CString((const char*)(aliases[i])));
	
	CAudioPage audioPage;
	Dlg.AddPage(&audioPage);

//        PString soundPlayCfgStr = config.GetString(SoundPlayConfigKey, "");
//	PString soundRecordCfgStr = config.GetString(SoundRecordConfigKey, "");

	PStringArray playDevices = PSoundChannel::GetDeviceNames(PSoundChannel::Player);
	for (i = 0; i < playDevices.GetSize(); i++)
        {
	 audioPage.m_PlayDeviceList.Add(CString((const char*)playDevices[i]));
//         if(soundPlayCfgStr == playDevices[i]) 
//          audioPage.m_PlayDeviceCur = CString((const char*)playDevices[i]);
	}   

        PString playerStr = m_endpoint.GetSoundChannelPlayDriver() + "\t" + m_endpoint.GetSoundChannelPlayDevice();
	audioPage.m_PlayDeviceCur = CString((const char*)playerStr);
	
	PStringArray recordDevices = PSoundChannel::GetDeviceNames(PSoundChannel::Recorder);
	for (i = 0; i < recordDevices.GetSize(); i++)
        {
	 audioPage.m_RecordDeviceList.Add(CString((const char*)recordDevices[i]));
//         if(soundRecordCfgStr == recordDevices[i]) 
//          audioPage.m_RecordDeviceCur = CString((const char*)recordDevices[i]);
	}

	PString recordStr = m_endpoint.GetSoundChannelRecordDriver() + "\t" + m_endpoint.GetSoundChannelRecordDevice();
	audioPage.m_RecordDeviceCur =  CString((const char*)recordStr);

	audioPage.m_SilenceDetect = m_endpoint.m_fSilenceOn ? TRUE : FALSE;
        audioPage.m_AECEnable = m_endpoint.m_fAECOn ? TRUE : FALSE;
        audioPage.m_AGCEnable = m_endpoint.m_fAGCOn;
	
	i = 0;
	for (;;) 
	{
		PString key = psprintf("%u", ++i);
		PString name = config.GetString(CodecsConfigSection, key, "");
		if (name.IsEmpty())
			break;
		audioPage.m_CodecList.Add((const char*)(name));
	}
	
	audioPage.m_Jitter = m_endpoint.GetMaxAudioJitterDelay();
	audioPage.m_uSoundBuffers = m_endpoint.GetSoundChannelBufferDepth();
	
	CVisualPage videoPage;
	Dlg.AddPage(&videoPage);
	
	PStringArray vdevices = PVideoInputDevice::GetDriversDeviceNames(CMyPhoneEndPoint::VideoInputDriver);
	for (i = 0; i < vdevices.GetSize(); i++)
		videoPage.m_RecordDeviceList.Add(CString((const char*)vdevices[i]));
	
	PStringArray fakedevice = PVideoInputDevice::GetDriversDeviceNames("FakeVideo");
	for (i = 0; i < fakedevice.GetSize(); i++)
		videoPage.m_RecordDeviceList.Add(CString((const char*)fakedevice[0]));

	PStringArray screendevice = PVideoInputDevice::GetDriversDeviceNames("ScreenVideo");
	for (i = 0; i < screendevice.GetSize(); i++)
		videoPage.m_RecordDeviceList.Add(CString((const char*)screendevice[i]));

	videoPage.m_RecordDeviceCur = CString((const char*)config.GetString(VideoDeviceConfigKey)); //YK: Should really be search in list
	if(videoPage.m_RecordDeviceCur.IsEmpty())
		videoPage.m_RecordDeviceCur = videoPage.m_RecordDeviceList[0];
	
	i = 0;
	for (;;) 
	{
		PString key = psprintf("%u", ++i);
		PString name = config.GetString(VideoCodecsConfigSection, key, "");
		if (name.IsEmpty())
			break;
		videoPage.m_CodecList.Add((const char*)(name));
	}
	//	videoPage.m_CodecList.Add("H.261"); //maggus: Need real list
	//	#if H323_VICH263
	//		videoPage.m_CodecList.Add("H.263"); //maggus: Need real list
	//	#endif
	
	videoPage.m_sendVideo = config.GetBoolean(AutoTransmitVideoConfigKey, TRUE);
	videoPage.m_receiveVideo = config.GetBoolean(AutoReceiveVideoConfigKey, TRUE);
	videoPage.m_localVideo = config.GetBoolean(VideoLocalConfigKey, TRUE);
	
	//	videoPage.m_LHflip = config.GetBoolean(VideoOutHFlipConfigKey, FALSE);
	videoPage.m_LVflip = config.GetBoolean(VideoOutVFlipConfigKey, FALSE);
	//	videoPage.m_RHflip = config.GetBoolean(VideoInHFlipConfigKey, FALSE);
	videoPage.m_RVflip = config.GetBoolean(VideoInVFlipConfigKey, FALSE);
	videoPage.m_localFlip = config.GetBoolean(VideoFlipLocalConfigKey, FALSE);
	
	videoPage.m_videoQTY = config.GetInteger(VideoQualityConfigKey, 15);
	videoPage.m_videoFPS = config.GetInteger(VideoFPSKey, 10);
	videoPage.m_videoInMaxBPS = config.GetInteger(VideoInMaxbandWidthKey, 320);
        videoPage.m_videoOutMaxBPS = config.GetInteger(VideoOutMaxbandWidthKey, 320);	

	videoPage.m_RecDevSrc = config.GetInteger(VideoSourceConfigKey, -1)+1;

//	int videoInSize = config.GetInteger(VideoInSizeConfigKey, 2);
//	videoPage.m_videoSize = videoInSize;
	videoPage.m_videoInSize = config.GetInteger(VideoInSizeConfigKey, 2);
	videoPage.m_videoOutSize = config.GetInteger(VideoOutSizeConfigKey, 2);
	// Disable Video Size choice during connection
//	if (m_endpoint.HasConnection(m_token) && m_endpoint.FindConnectionWithLock(m_token) != NULL)
//		videoPage.m_showVideoSize = FALSE; 

	// GUI settings
	CguiPage guiPage;
	Dlg.AddPage(&guiPage);
	
	guiPage.clrInMsg = colInmsg;
	guiPage.clrOutMsg = colOutmsg;
	guiPage.clrActInd = CMyIndicator::clrFg;
	guiPage.clrSlndInd = CMyIndicator::clrSFg;
	
	guiPage.m_vidhidechk = autohideVideoPan;
	guiPage.m_stathidechk = hideStat;
	guiPage.m_loghidechk = hideSysMsg;
	guiPage.m_autoaddadrchk = autoAddInAddr;
	guiPage.m_fullscrdflt = fullScreenDefault;
	guiPage.m_lang = config.GetInteger(UILangugeConfigKey, -1)+1;
	
	// changing some styles of the prop window
	Dlg.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	Dlg.m_psh.dwFlags ^= PSH_HASHELP;
	if(Dlg.DoModal() != IDOK)      //	****		 show settings dialog window	****
		return;
	
	CMyPhoneDlg::colInmsg = guiPage.clrInMsg;
	config.SetInteger(ColorInMessageConfigKey, guiPage.clrInMsg);
	CMyPhoneDlg::colOutmsg = guiPage.clrOutMsg;
	config.SetInteger(ColorOutMessageConfigKey, guiPage.clrOutMsg);
	CMyIndicator::clrFg = guiPage.clrActInd;
	config.SetInteger(ColorSignalIndicatorConfigKey, guiPage.clrActInd);
	CMyIndicator::clrSFg = guiPage.clrSlndInd;
	config.SetInteger(ColorSilentIndicatorConfigKey, guiPage.clrSlndInd);
	
	autohideVideoPan = guiPage.m_vidhidechk;
	config.SetBoolean(AutoVideoHideConfigKey, guiPage.m_vidhidechk);
	if(autohideVideoPan && showVideoPan && (!m_endpoint.HasConnection(m_token)))
		ShowVideoPanels(FALSE);
	hideStat = guiPage.m_stathidechk;
	config.SetBoolean(HideStatConfigKey, guiPage.m_stathidechk); // show statistic info
	fullScreenDefault = guiPage.m_fullscrdflt;
	config.SetBoolean(FullScreenDefaultConfigKey, guiPage.m_fullscrdflt);
	hideSysMsg = guiPage.m_loghidechk;
	config.SetBoolean(SysLogMsgHideConfigKey, guiPage.m_loghidechk);
	autoAddInAddr = guiPage.m_autoaddadrchk; 
	config.SetBoolean(AutoAddCallersConfigKey, guiPage.m_autoaddadrchk);
	config.SetInteger(UILangugeConfigKey, guiPage.m_lang -1);
	curLang = GetLangFromTable(guiPage.m_lang -1);	// Setting Languge
	
	PString str(audioPage.m_PlayDeviceCur);
	config.SetString(SoundPlayConfigKey, str);
//	m_endpoint.SetSoundChannelPlayDevice(str);

        PStringArray soundPlayer = str.Tokenise('\t');
        if(soundPlayer.GetSize()==1)
        {  
         m_endpoint.SetSoundChannelPlayDriver("WindowsMutimedia");
	 m_endpoint.SetSoundChannelPlayDevice(soundPlayer[0]);
         PTRACE(1, "MyPhone\tSetSoundPlayer Driver:WindowsMultimedia, Device:" << soundPlayer[0]);
        }
        else
        {
         m_endpoint.SetSoundChannelPlayDriver(soundPlayer[0]);
	 m_endpoint.SetSoundChannelPlayDevice(soundPlayer[1]);
         PTRACE(1, "MyPhone\tSetSoundPlayer Driver:" << soundPlayer[0] << ", Device:" << soundPlayer[1]);
	}


	str = PString(audioPage.m_RecordDeviceCur);
	config.SetString(SoundRecordConfigKey, str);
//	m_endpoint.SetSoundChannelRecordDevice(str);

        PStringArray soundRecorder = str.Tokenise('\t');
        if(soundRecorder.GetSize()==1)
        {  
         m_endpoint.SetSoundChannelRecordDriver("WindowsMutimedia");
	 m_endpoint.SetSoundChannelRecordDevice(soundRecorder[0]);
         PTRACE(1, "MyPhone\tSetSoundRecorder Driver:WindowsMultimedia, Device:" << soundRecorder[0]);
        }
        else
        {
         m_endpoint.SetSoundChannelRecordDriver(soundRecorder[0]);
	 m_endpoint.SetSoundChannelRecordDevice(soundRecorder[1]);
         PTRACE(1, "MyPhone\tSetSoundRecorder Driver:" << soundRecorder[0] << ", Device:" << soundRecorder[1]);
	}

	
	
	m_endpoint.m_fSilenceOn = audioPage.m_SilenceDetect;
        m_endpoint.m_fAECOn = audioPage.m_AECEnable;
        m_endpoint.m_fAGCOn = audioPage.m_AGCEnable;
	m_endpoint.SetSilenceDetectionMode(m_endpoint.m_fSilenceOn ? 
		H323AudioCodec::AdaptiveSilenceDetection :
	H323AudioCodec::NoSilenceDetection);
        m_endpoint.SetAECAlgo(m_endpoint.m_fAECOn);
        m_endpoint.agc = m_endpoint.m_fAGCOn;
	config.SetBoolean(SilenceDetectConfigKey, audioPage.m_SilenceDetect);
        config.SetBoolean(AECEnableConfigKey, audioPage.m_AECEnable);
        config.SetInteger(AGCEnableConfigKey, audioPage.m_AGCEnable);
	// Setting current connection
	if (m_endpoint.HasConnection(m_token)) {
		H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
		if (connection != NULL) {
			H323Channel * channel = connection->FindChannel(RTP_Session::DefaultAudioSessionID, FALSE);
			if (channel != NULL) {
				H323Codec * codec = channel->GetCodec();
				if (PIsDescendant(codec,H323AudioCodec))
					((H323AudioCodec*)codec)->SetSilenceDetectionMode(
					m_endpoint.GetSilenceDetectionMode(),
					0, 80 /*default*/, 3200 /*default*/);
			}
			connection->Unlock();
		}
	}
	
	m_endpoint.SetAudioJitterDelay(50, audioPage.m_Jitter);
	config.SetInteger(JitterConfigKey, audioPage.m_Jitter);
	
	m_endpoint.SetSoundChannelBufferDepth(audioPage.m_uSoundBuffers);
	config.SetInteger(BufferCountConfigKey, audioPage.m_uSoundBuffers);
	
	// refreshing Audio Codecs
	PString defaultSection = config.GetDefaultSection();
	config.SetDefaultSection(CodecsConfigSection);
	config.DeleteSection();
	for (i = 0; i < audioPage.m_CodecList.GetSize(); i++)
		config.SetString(psprintf("%u", i+1), PString(audioPage.m_CodecList[i]));
	config.SetDefaultSection(defaultSection);
	
	
	BOOL gkChanged = (config.GetString(GatekeeperHostConfigKey) != PString(gatePage.m_gkhost)) ||
		(config.GetInteger(DiscoverGatekeeperConfigKey) != gatePage.m_gkdiscover) ||
		(config.GetBoolean(UseGatekeeperConfigKey) != gatePage.m_gkuse);
	
	config.SetBoolean(UseGatekeeperConfigKey, gatePage.m_gkuse);
	config.SetBoolean(RequireGatekeeperConfigKey, gatePage.m_gkrec);
	
	config.SetString(GatekeeperHostConfigKey, PString(gatePage.m_gkhost));
	//	config.SetString(GatewayHostConfigKey, PString(gatePage.m_GatewayHost));
	config.SetString(GatekeeperPassConfigKey, PString(gatePage.m_gkpass));
	
	config.SetInteger(DiscoverGatekeeperConfigKey, gatePage.m_gkdiscover);
	//	config.SetBoolean(DiscoverGatewayConfigKey, gatePage.m_DiscoverGateway);
	
	m_endpoint.SetLocalUserName(PString(genPage.m_UserName));
	config.SetString(UsernameConfigKey, PString((LPCTSTR)(genPage.m_UserName)));
	
	CString strAlias;
	config.DeleteKey(AliasConfigKey);
	for (i = 0; i < gatePage.m_AliasList.GetSize(); i++) 
	{
		strAlias += gatePage.m_AliasList.GetAt(i)+"|";
		m_endpoint.AddAliasName((LPCTSTR) gatePage.m_AliasList.GetAt(i));
	}
	config.SetString(AliasConfigKey, (LPCTSTR) strAlias);
	
	m_endpoint.m_fAutoAnswer = genPage.m_AutoAnswer;
	config.SetBoolean(AutoAnswerConfigKey, genPage.m_AutoAnswer);
	
	m_endpoint.m_fDtmfAsString = genPage.m_DtmfAsString;
	config.SetBoolean(DtmfAsStringConfigKey, genPage.m_DtmfAsString);
	
	m_endpoint.DisableH245Tunnelling( genPage.m_NoTunneling );
	config.SetBoolean(NoTunnelingConfigKey, genPage.m_NoTunneling);
	
	m_endpoint.m_fNoFastStart = genPage.m_disableFastStart;
	config.SetBoolean(NoFastStartConfigKey, genPage.m_disableFastStart);
	
	m_endpoint.m_fStrictSingleLine = genPage.m_singleLine;
	config.SetBoolean(StrictSingleLineConfigKey, genPage.m_singleLine);

	if( !genPage.m_RingSoundFile.IsEmpty() )
	{
		ringSoundFile = PString((LPCTSTR) genPage.m_RingSoundFile);
		config.SetString(RingSoundFileConfigKey, ringSoundFile);
	}
	else
	{
		ringSoundFile = _T("");
		config.DeleteKey(RingSoundFileConfigKey);
	}
	
	// Networking
	m_endpoint.SetInitialBandwidth((unsigned)(netPage.m_Bandwidth*20));
	config.SetReal(BandwidthConfigKey, netPage.m_Bandwidth);
	config.SetInteger(BandwidthTypeConfigKey, netPage.m_BandwidthType);
	m_endpoint.SetRtpIpTypeofService(netPage.m_IP_Type);
	config.SetInteger(IpTosConfigKey, netPage.m_IP_Type);
	config.SetInteger(RTPPortBaseConfigKey, netPage.m_rtpbase);
	config.SetInteger(RTPPortMaxConfigKey, netPage.m_rtpmax);
	m_endpoint.SetRtpIpPorts(netPage.m_rtpbase, netPage.m_rtpmax);
	config.SetString(RouterConfigKey, (LPCTSTR)netPage.m_natip);
	m_endpoint.m_router = (LPCTSTR)netPage.m_natip;
	config.SetString(ListenerInterfaceConfigKey, (LPCTSTR)netPage.m_linterface);
	PString lifaces_new = (LPCTSTR)netPage.m_linterface;
	if(lifaces_new != lifaces)
	{
		AfxGetApp()->DoWaitCursor(1);
		if (m_endpoint.StartListeners(lifaces_new.Tokenise(',')))
			OutputStatusStr((LPCTSTR)LoadStringLang(IDS_MPREADY1STR), S_SYSTEM, (const char *)strProgName);
		else
			OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRLSNSTR), S_SYSTEM, (const char *)strProgName, (const char *)lifaces_new);		
		AfxGetApp()->DoWaitCursor(0); 
	}
	
	// Video stuff
	str = PString(videoPage.m_RecordDeviceCur);
	config.SetString(VideoDeviceConfigKey, str);
	config.SetInteger(VideoSourceConfigKey, videoPage.m_RecDevSrc -1);

	config.SetInteger(VideoInSizeConfigKey, videoPage.m_videoInSize);
	config.SetInteger(VideoOutSizeConfigKey, videoPage.m_videoOutSize);

	m_endpoint.autoStartTransmitVideo = videoPage.m_sendVideo;
	config.SetBoolean(AutoTransmitVideoConfigKey, videoPage.m_sendVideo);
	
	m_endpoint.autoStartReceiveVideo = videoPage.m_receiveVideo;
	config.SetBoolean(AutoReceiveVideoConfigKey, videoPage.m_receiveVideo);
	
	m_endpoint.localVideo = videoPage.m_localVideo;
	config.SetBoolean(VideoLocalConfigKey, videoPage.m_localVideo);
	
	// Codecs in separate section
	defaultSection = config.GetDefaultSection();
	config.SetDefaultSection(VideoCodecsConfigSection);
	config.DeleteSection();
	for (i = 0; i < videoPage.m_CodecList.GetSize(); i++)
		config.SetString(psprintf("%u", i+1), PString(videoPage.m_CodecList[i]));
	config.SetDefaultSection(defaultSection);
	
	//	config.SetBoolean(VideoOutHFlipConfigKey, videoPage.m_LHflip);
	config.SetBoolean(VideoOutVFlipConfigKey, videoPage.m_LVflip);
	//	config.SetBoolean(VideoInHFlipConfigKey, videoPage.m_RHflip);
	config.SetBoolean(VideoInVFlipConfigKey, videoPage.m_RVflip);
	config.SetBoolean(VideoFlipLocalConfigKey, videoPage.m_localFlip);
	m_endpoint.localFlip = videoPage.m_localFlip;
	
	config.SetInteger(VideoQualityConfigKey, videoPage.m_videoQTY);
	config.SetInteger(VideoFPSKey, videoPage.m_videoFPS);
	config.SetInteger(VideoInMaxbandWidthKey, videoPage.m_videoInMaxBPS);
        config.SetInteger(VideoOutMaxbandWidthKey, videoPage.m_videoOutMaxBPS);
	// Change langage     partial translation, restart neded for full one
	TranslateDlg();
	// Change Video panals size
//	int size = config.GetInteger(VideoInSizeConfigKey, 2);
//	if(size == 2)
//		changeVideoSize(QCIF);
//	else if(size == 3)
//		changeVideoSize(CIF);
	// Reload Endpoint capabilities
	m_endpoint.LoadCapabilities();
	// Reregister with gatekeeper if settings changed
	if( gkChanged ) 
		PostMessage(WM_FINDGATEKEEPER);
}

//CVideoDlg *vdlg;

void CMyPhoneDlg::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

//vdlg=new CVideoDlg();
//vdlg->Create(IDD_VIDEOBOX,NULL);
//vdlg->ShowWindow(SW_SHOW);
//   CWnd* pWnd = new CStatic;
//   pWnd->CreateEx(WS_EX_OVERLAPPEDWINDOW, // Make a client edge label.
//      _T("STATIC"), "Hi",
//      WS_OVERLAPPED | WS_VISIBLE,
//      5, 5, 300, 300, NULL, NULL);
//pWnd->EnableWindow(TRUE);

}

void CMyPhoneDlg::OnAppHelp()
{
	ShellExecute(NULL, "open", strProgWebHelp, NULL, NULL, SW_SHOWNORMAL);
}

void CMyPhoneDlg::OnClose() 
{
	//    if(WaitForSingleObject( hCloseMutex, 3000L)!=WAIT_OBJECT_0)
	//	{
	//		AfxMessageBox("Video output sinchronization failed!");
	//	}
	if(m_endpoint.HasConnection(m_token))
		m_endpoint.ClearCall(m_token);
	//	ReleaseMutex(hCloseMutex);
	// Restore old volume
	//	waveOutSetVolume(0, m_sliderVolume.GetPos());	
	//	SetVolume(MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT,
	//		MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT, 
	//		m_sliderVolume.GetPos());
	// in volume
	//	SetVolume(MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
	//		MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,
	//		m_sliderMic.GetPos());
	
	RemoveIcon();
	//MessageBox("OnClose");
	// Closing
	CDialog::OnClose();
}

BOOL CMyPhoneDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if( IDC_SLIDER_VOLUME == (int) wParam )
		//waveOutSetVolume(0, m_sliderVolume.GetPos());
		SetVolume(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS ,
		MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT, 
		m_sliderVolume.GetPos());
	
	if( IDC_SLIDER_MIC == (int) wParam )
		SetVolume(MIXERLINE_COMPONENTTYPE_DST_WAVEIN,
		MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,
		m_sliderMic.GetPos());
	// RichEdit mouse events
	MSGFILTER * lpMsgFilter = (MSGFILTER *)lParam; 
	
	if ((wParam == IDC_STATUS) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER)   
		&& (lpMsgFilter->msg == WM_RBUTTONDOWN))                                       
		
	{
		// if we get through here, 
		// we have trapped the right click event of the richeditctrl! 
		CPoint point;	                                        
		::GetCursorPos(&point); //where is the mouse?
		CMenu menu; //lets display out context menu :) 
		UINT dwSelectionMade;                                       
		VERIFY(menu.LoadMenu(IDR_LOGMENU) );  
		CMenu *pmenuPopup = menu.GetSubMenu(0);
		ASSERT(pmenuPopup != NULL);                                       
		dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
			point.x, point.y, this);                                
		pmenuPopup->DestroyMenu(); 
		PostMessage(dwSelectionMade,0,0);
	}
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

LRESULT CMyPhoneDlg::OnLogSelectAll(WPARAM, LPARAM)
{	// menu actions from log RichEdit 
	m_cstatus.SetSel(0,-1);
	return TRUE;
}

LRESULT CMyPhoneDlg::OnLogCopy(WPARAM, LPARAM)
{	// menu actions from log RichEdit 
	m_cstatus.Copy();
	return TRUE;
}

LRESULT CMyPhoneDlg::OnLogClear(WPARAM, LPARAM)
{	// menu actions from log RichEdit 
	m_cstatus.SetWindowText(_T(""));
	OutputStatusStr((LPCTSTR) LoadStringLang(IDS_MPREADYSTR), S_SYSTEM, (const char *)strProgName);
	return TRUE;
}
//
LRESULT CMyPhoneDlg::OnDestCopy(WPARAM, LPARAM)
{	// menu actions from Destination combobox 
	m_cdestination.Copy();
	return TRUE;
}

LRESULT CMyPhoneDlg::OnDestPaste(WPARAM, LPARAM)
{	// menu actions from log Destination combobox 
	m_cdestination.SetEditSel(0, -1);
	m_cdestination.Paste();
	return TRUE;
}

LRESULT CMyPhoneDlg::OnDestClear(WPARAM, LPARAM)
{	// menu actions from log Destination combobox 
	m_endpoint.config.DeleteSection(AddressBookSection);
	m_cdestination.ResetContent();
	m_cdestination.AddString(_T("127.0.0.1 - localhost"));
	OutputStatusStr((LPCTSTR) LoadStringLang(IDS_DSTCLEAREDSTR), S_SYSTEM);
	return TRUE;
}
///////////////////////////
// Audio Mixers functions
//
bool CMyPhoneDlg::SetVolume(DWORD inout, DWORD ComponentType, DWORD dwVol)
{
/*	HMIXER hMixer;
	HRESULT hr;
	hr = mixerOpen(&hMixer, 0, 0, 0, 0);
	if (FAILED(hr)) false;
	
	MIXERLINE mxl;
	MIXERCONTROL mc;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED mxdu;
	DWORD count, armxdu[2];
	
	memset(&mxl, 0, sizeof(mxl));
	mxl.cbStruct = sizeof(mxl);
	mxl.dwComponentType = inout;
	
    hr = mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);
	if (FAILED(hr) || mxl.cControls == 0)
	{
		mixerClose(hMixer);
		return false;
	}
	
	count = mxl.cConnections;
	if(count == (UINT) -1)
	{
		mixerClose(hMixer);
		return false;
	}
	
	for(UINT i = 0; i < count; i++)
	{
		mxl.dwSource = i;
		mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl, MIXER_GETLINEINFOF_SOURCE);
		if (mxl.dwComponentType == ComponentType)
		{
			mc.cbStruct = sizeof(mc);
			mxlc.cbStruct = sizeof(mxlc);
			mxlc.dwLineID = mxl.dwLineID;
			mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
			mxlc.cControls = 1;
			mxlc.cbmxctrl = sizeof(MIXERCONTROL);
			mxlc.pamxctrl = &mc;
			hr = mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);
			// setting value
			for(UINT i=0; i<(mxl.cChannels); i++)
				armxdu[i]=dwVol;
			mxdu.dwValue = dwVol;
			mxcd.cMultipleItems = 0;
			mxcd.cChannels = mxl.cChannels;
			mxcd.cbStruct = sizeof(mxcd);
			mxcd.dwControlID = mc.dwControlID;
			mxcd.cbDetails = sizeof(armxdu);
			mxcd.paDetails = &armxdu;
			hr = mixerSetControlDetails((HMIXEROBJ)hMixer, &mxcd, MIXER_SETCONTROLDETAILSF_VALUE);	
			break;
		}
	}
	mixerClose(hMixer); */
	return true;
}

DWORD CMyPhoneDlg::GetVolume(DWORD inout, DWORD ComponentType)
{
/*	PTRACE(4, "MyPhone\t###GetVolume###\tEnter GetVolume function");
	//
	HMIXER hMixer;
	HRESULT hr;
	//
	PTRACE(4, "MyPhone\t###GetVolume###\tmixerOpen");
	//
	hr = mixerOpen(&hMixer, 0, 0, 0, 0);
	if (FAILED(hr))
	{
		PTRACE(4, "MyPhone\t###GetVolume###\tmixerOpen failed!");
		return -1;
	}
	
	MIXERLINE mxl;
	MIXERCONTROL mc;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED mxdu;
	DWORD count, armxdu[]={0L, 0L};
	
	memset(&mxl, 0, sizeof(mxl));
	mxl.cbStruct = sizeof(mxl);
	mxl.dwComponentType = inout /*ComponentType*/;
/*	mxdu.dwValue = -1;	// default
	
	PTRACE(4, "MyPhone\t###GetVolume###\tmixerGetLineInfo");
	
    hr = mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);
	if (FAILED(hr) || mxl.cControls==0)
	{
		mixerClose(hMixer);
		PTRACE(4, "MyPhone\t###GetVolume###\tmixerGetLineInfo failed!");
		return -1;
	}
	
	count = mxl.cConnections/* dwSource*/;
/*	if(count == (UINT)-1)
	{
		mixerClose(hMixer);
		PTRACE(4, "MyPhone\t###GetVolume###\tNo such audio mixer channel!");
		return -1;
	}
	//
	CString strMsg;
	strMsg.Format("MyPhone\t###GetVolume###\tmixerGetLineInfo found sources: %d", count);
	PTRACE(4, (LPCTSTR)strMsg);
	//
	for(UINT i = 0; i < count; i++)
	{
		mxl.dwSource = i;
		//
		CString strMsg;
		strMsg.Format("MyPhone\t###GetVolume###\tmixerGetLineInfo for source: %d", i);
		PTRACE(4, (LPCTSTR)strMsg);
		//
		hr = mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl, MIXER_GETLINEINFOF_SOURCE);
		if ((!FAILED(hr)) && (mxl.dwComponentType == ComponentType))
		{
			mc.cbStruct = sizeof(mc);
			mxlc.cbStruct = sizeof(mxlc);
			mxlc.dwLineID = mxl.dwLineID;
			mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
			mxlc.cControls = 1;
			mxlc.cbmxctrl = sizeof(MIXERCONTROL);
			mxlc.pamxctrl = &mc;
			//
			CString strMsg;
			strMsg.Format("MyPhone\t###GetVolume###\tmixerGetLineControls for line: %s", (LPCTSTR)mxl.szName);
			PTRACE(4, (LPCTSTR)strMsg);
			//
			
			hr = mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);
			if(FAILED(hr))
			{
				PTRACE(4, "MyPhone\t###GetVolume###\tmixerGetLineControls failed!");
				continue;
			}
			// getting value
			mxcd.cMultipleItems = 0;
			mxcd.cChannels = mxl.cChannels;
			mxcd.cbStruct = sizeof(mxcd);
			mxcd.dwControlID = mc.dwControlID;
			mxcd.cbDetails = sizeof(mxdu);
			mxcd.paDetails = &mxdu;
			hr = mixerGetControlDetails((HMIXEROBJ)hMixer, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);	
			if(FAILED(hr))
				PTRACE(4, "MyPhone\t###GetVolume###\tmixerGetControlDetails failed!");
			break;
		}
	}
	mixerClose(hMixer);
	
	PTRACE(4, "MyPhone\t###GetVolume###\tExiting function"); */
	return true; //mxdu.dwValue;
}
///////////////////////////

void CMyPhoneDlg::SendUserInput(LPCTSTR strusermsg)
{
	H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
	if (connection == NULL)
		return;
	
	connection->SendUserInput(PString(strusermsg));
	connection->Unlock();
	
	OutputStatusStr((LPCTSTR) LoadStringLang(IDS_USAIDMSGSTR), S_OUTMSG, strusermsg);
}

BOOL CMyPhoneDlg::PreTranslateMessage(MSG* pMsg) 
{


	if (pMsg->hwnd==GetDlgItem(IDC_USERMSG)->GetSafeHwnd() &&
		pMsg->message==WM_KEYDOWN &&
		pMsg->wParam==VK_RETURN && 
		(GetKeyState(VK_CONTROL)<0))
		// Catching "Ctrl+Enter" in Message edit
	{
		CString newstr;
		m_usermsg.GetWindowText(newstr);
		newstr.TrimRight();
		newstr.TrimLeft();
		if(!(newstr.IsEmpty()))
			SendUserInput(newstr.GetBuffer(0));
		m_usermsg.SetWindowText(_T(""));
		return TRUE;
	}
	else if (pMsg->hwnd==GetDlgItem(IDC_DESTINATION)->GetWindow(GW_CHILD)->GetSafeHwnd() && 
		pMsg->message==WM_KEYDOWN &&
		pMsg->wParam==VK_RETURN) 
		// Catching "Enter" in Destination ComboBox
	{
		OnCall(); 
		return TRUE;
	}
	else if (pMsg->hwnd==GetDlgItem(IDC_DESTINATION)->GetWindow(GW_CHILD)->GetSafeHwnd() && 
		//	GetFocus()==GetDlgItem(IDC_DESTINATION)->GetWindow(GW_CHILD) &&
		pMsg->message ==WM_RBUTTONDOWN)
		// Adding custom menu to Destination ComboBox
	{
		m_cdestination.SetFocus();
		CMenu menu;
        DWORD dwSelectionMade; 
		VERIFY(menu.LoadMenu(IDR_DESTMENU));
        CMenu *pmenuPopup = menu.GetSubMenu(0);
        ASSERT(pmenuPopup != NULL);                                    
        dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|
			TPM_LEFTBUTTON|
			TPM_NONOTIFY|
			TPM_RETURNCMD),
			pMsg->pt.x, 
			pMsg->pt.y, this
			);  
        pmenuPopup->DestroyMenu();
        PostMessage(dwSelectionMade,0,0);
        return TRUE;
    }
	//	else if(pMsg->hwnd==this->GetSafeHwnd() && 
	//		pMsg->message==SC_CLOSE) 
	//		// Catching "Close" in Main Window
	//	{
	//		MessageBox("Pretranslate"); 
	//		return TRUE;
	//	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////
// calles from EndPoint
//
BOOL CMyPhoneDlg::OnAnswerCall(const H323Connection & connection)
{
	PString caller = FindContactName(connection)/*connection.GetRemotePartyName()*/;
	m_token = connection.GetCallToken();
    m_caller.SetWindowText(CString((const char*)caller) + LoadStringLang(IDS_CALLINGSTR));
    //m_answer.EnableWindow();
    m_refuse.EnableWindow();
    m_call.EnableWindow(TRUE);
	GetDlgItem(IDC_CALL)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ANSWERBSTR));
	GetDlgItem(IDC_REFUSE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_REFUSEBSTR));
    OutputStatusStr((LPCTSTR)LoadStringLang(IDS_CALLING1STR), S_SYSTEM, (const char *)caller);
	PTime now;
	PString nowStr = now.AsString("w h:m a");
	
	// Check for auto answer option
	if (m_endpoint.m_fAutoAnswer)
	{
		ringSoundTimer.Stop();
		return TRUE;
	}
	
	if (!ringSoundFile) 
	{
		PSound::PlayFile(ringSoundFile, FALSE);
		ringSoundTimer.RunContinuous(5000);
	}
	
	//	if (!noAnswerForwardParty)
	//		noAnswerTimer = noAnswerTime;
	return FALSE;
}

void CMyPhoneDlg::OnRingSoundAgain(PTimer &, INT)
{
	if (!ringSoundFile)
		PSound::PlayFile(ringSoundFile, FALSE);
	ChangeIcon(CString(""));
}


void CMyPhoneDlg::StoreDestonation(LPCTSTR destName, BOOL isIncoming)
{
	// storing good destination
	int curPos=-1, MAXDESTHIST=10;
	CString curDest, strName, strDestName;
	if(isIncoming)
	{
		CString tmpStr = CString(destName);
		tmpStr.TrimLeft();
		int iLpos=0, iRpos=0;
		if((curPos=tmpStr.Find("["))>0)
		{
			strName = tmpStr.Left(curPos);
			strName += _T(" (auto added)");
			iLpos = curPos;
		}
		if((curPos=tmpStr.Find("]"))>0)
			iRpos = curPos;
		if((iRpos>=0) && (iLpos>=0) && (iRpos>iLpos))
			curDest = tmpStr.Mid(iLpos+1, iRpos-iLpos-1);
	}
	else
	{
		if(m_destination.IsEmpty())
			return;
		curDest = m_destination;
		curDest.TrimLeft();
		strName = CString(destName);
		strName.TrimLeft();
		if((curPos=curDest.Find(" "))>0)
			curDest = curDest.Left(curPos);
		if((curPos=strName.Find("["))>0)
			strName = strName.Left(curPos);
	}
	strDestName = curDest+_T(" - ")+strName;
    if((curPos=m_cdestination.FindString( 0, curDest.GetBuffer(0)))==LB_ERR)
	{
		m_cdestination.InsertString(0, strDestName.GetBuffer(0));
	}
	else
	{
		m_cdestination.DeleteString(curPos);
		m_cdestination.InsertString(curPos, strDestName.GetBuffer(0));
	}
	while(m_cdestination.GetCount()>MAXDESTHIST)
		m_cdestination.DeleteString(m_cdestination.GetCount()-1);
	// store to config
	PConfig *m_cfg = &m_endpoint.config;
	CString strNew(""), strLine("");
	int len;
	m_cfg->DeleteSection(AddressBookSection); 
	for(int i=0; i<m_cdestination.GetCount(); i++)
	{
		len = m_cdestination.GetLBTextLen( i );
		if(!len) continue;
		m_cdestination.GetLBText(i, strLine.GetBuffer(len));
		strLine.ReleaseBuffer();
		strNew+=strLine+CString("|");
	}
	m_cfg->SetString(AddressBookSection, AddressBookRescentKey, strNew.GetBuffer(0));
	m_cdestination.SelectString(0, curDest ); //update edit part
}
void CMyPhoneDlg::LoadDestonation()
{
	PConfig *m_cfg = &m_endpoint.config;
	if(m_cfg->HasKey(AddressBookSection, AddressBookRescentKey))
		m_cdestination.ResetContent();
	else
		return;
	CString strDest = CString((LPCTSTR) m_cfg->GetString(AddressBookSection, AddressBookRescentKey, ""));
	while(strDest.Find("|")>=0)
	{
		m_cdestination.AddString(strDest.Left(strDest.Find("|")));
		strDest.Delete(0, strDest.Find("|")+1);
	}
}

void CMyPhoneDlg::ShowStats()
{
	// Getting statistics in a passive way, through callback.
	// It seams Not to include Video statistic, dont know why?
	CString sStats;
	CTime tCll((long int)m_endpoint.m_stat.iSecs);
	if((!hideStat) && m_endpoint.HasConnection(m_token) && (m_endpoint.m_stat.iSecs>0) && (m_endpoint.m_stat.iSecs<9999) )
	{
		if(!m_endpoint.m_StatLock.Lock(10)) return; //no lock
		if(showVideoPan)
		{
			sStats.Format((LPCTSTR)LoadStringLang(IDS_STATFSTR), 
				tCll.FormatGmt("%H:%M:%S"), 
				ParseBytes(m_endpoint.m_stat.ibSent).GetBuffer(0), 
				ParseBytes((m_endpoint.m_stat.ibSent)/(m_endpoint.m_stat.iSecs>0?m_endpoint.m_stat.iSecs:1)).GetBuffer(0),
				ParseBytes(m_endpoint.m_stat.ibRcvd).GetBuffer(0),
				ParseBytes((m_endpoint.m_stat.ibRcvd)/(m_endpoint.m_stat.iSecs>0?m_endpoint.m_stat.iSecs:1)).GetBuffer(0),
				m_endpoint.m_stat.iDelay );
		}
		else
		{
			sStats.Format((LPCTSTR)LoadStringLang(IDS_STATSSTR), 
				tCll.FormatGmt("%#H:%#M:%#S"), 
				ParseBytes(m_endpoint.m_stat.ibSent).GetBuffer(0), 
				ParseBytes((m_endpoint.m_stat.ibSent)/(m_endpoint.m_stat.iSecs>0?m_endpoint.m_stat.iSecs:1)).GetBuffer(0),
				ParseBytes(m_endpoint.m_stat.ibRcvd).GetBuffer(0),
				ParseBytes((m_endpoint.m_stat.ibRcvd)/(m_endpoint.m_stat.iSecs>0?m_endpoint.m_stat.iSecs:1)).GetBuffer(0),
				m_endpoint.m_stat.iDelay );
		}
		//m_bar.SetPaneText(0,sStats.GetBuffer(0));
		m_endpoint.m_StatLock.Unlock();
		m_statbar.SetWindowText(sStats.GetBuffer(0));
	}
	else
		//m_bar.SetPaneText(0,m_oldStatus.GetBuffer(0));
		m_statbar.SetWindowText(m_oldStatus.GetBuffer(0));
	//	Not getting statistic actively because it halts connection sometimes.
	/*	H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
	if (connection == NULL)
	{
	m_bar.SetPaneText(0,m_oldStatus.GetBuffer(0));
	return;
	}
	
	  CString sStats;
	  // Getting Audio statistic
	  RTP_Session * session = connection->GetSession(RTP_Session::DefaultAudioSessionID);
	  if (session != NULL)
	  {
	  m_endpoint.OnRTPStatistics(*connection, *session);
	  
		CTime tCll(m_endpoint.m_stat.iSecs); 
		sStats.Format("In call: %s   Sent: %s(%s/s)   Received: %s(%s/s)   Delay: %d ms", 
		tCll.Format("%H:%M:%S"), 
		ParseBytes(m_endpoint.m_stat.ibSent).GetBuffer(0), 
		ParseBytes((m_endpoint.m_stat.ibSent)/(m_endpoint.m_stat.iSecs>0?m_endpoint.m_stat.iSecs:1)).GetBuffer(0),
		ParseBytes(m_endpoint.m_stat.ibRcvd).GetBuffer(0),
		ParseBytes((m_endpoint.m_stat.ibRcvd)/(m_endpoint.m_stat.iSecs>0?m_endpoint.m_stat.iSecs:1)).GetBuffer(0),
		m_endpoint.m_stat.iDelay );
		
		  }
		  connection->Unlock();
		  // Setting Test to Pannels
		  m_bar.SetPaneText(0,sStats.GetBuffer(0));
	*/
}

void CMyPhoneDlg::OnBvidpan() 
{
	ShowVideoPanels(!showVideoPan);
}

void CMyPhoneDlg::OnTimer(UINT nIDEvent) 
{
	if((nIDEvent==100) && (!hideStat))	
	{
		ShowStats();
		//		OnUpdateIndicators();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMyPhoneDlg::OnUpdateIndicators(PTimer &, INT)
{
	H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
	if (connection == NULL)
		return;
	
	H323Channel * channel = connection->FindChannel(RTP_Session::DefaultAudioSessionID, TRUE);
	if (channel != NULL)
	{
		H323AudioCodec * codec = (H323AudioCodec *)channel->GetCodec();
		m_volindicator.SetCurrentLevel(codec->GetAverageSignalLevel());
	}
	
	channel = connection->FindChannel(RTP_Session::DefaultAudioSessionID, FALSE);
	if (channel != NULL)
	{
		H323AudioCodec * codec = (H323AudioCodec *)channel->GetCodec();
		BOOL inTalkBurst;
		unsigned threshold;
		if (codec->GetSilenceDetectionMode(&inTalkBurst, &threshold) != H323AudioCodec::NoSilenceDetection)
		{
			m_micindicator.SetMarkerLevel(threshold);
			m_micindicator.SetActive(inTalkBurst);
		}
		m_micindicator.SetCurrentLevel(codec->GetAverageSignalLevel());
	}
	connection->Unlock();
}

void CMyPhoneDlg::ShowVideoPanels(bool show)
{
	// showing and hiding video panels
	if(IsIconic()) return;
	int iVidpan = curVideoSize==CIF?203+176:203;
	RECT wndPos;
	if(show)
	{
		// dialog window
		GetWindowRect( &wndPos );
		wndPos.right += iVidpan;
		MoveWindow(&wndPos);
		// button 
		m_vidPan.GetWindowRect( &wndPos );
		ScreenToClient(&wndPos);
		wndPos.left += iVidpan;
		wndPos.right += iVidpan;
		m_vidPan.MoveWindow(&wndPos);
		//		m_vidPan.SetWindowText("<");
		m_vidPan.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LARRICON)));	
		// status bar
		//		GetClientRect(&wndPos);
		//		m_bar.SetPaneInfo(0,ID_INDICATOR_IP,SBPS_NORMAL, wndPos.right-50);
		m_statbar.GetWindowRect( &wndPos );
		ScreenToClient(&wndPos);
		wndPos.right += iVidpan;
		m_statbar.MoveWindow(&wndPos);
		// menu bar
		//		SetMainMenu(FALSE, iVidpan);
		//
		showVideoPan=true;
	}
	else
	{
		// Menu bar
		//		SetMainMenu(FALSE, -iVidpan);
		// Dialog window
		GetWindowRect( &wndPos );
		wndPos.right -= iVidpan;
		MoveWindow(&wndPos);
		// button
		m_vidPan.GetWindowRect( &wndPos );
		ScreenToClient(&wndPos);
		wndPos.left -= iVidpan;
		wndPos.right -= iVidpan;
		m_vidPan.MoveWindow(&wndPos);
		//		m_vidPan.SetWindowText(">");
		m_vidPan.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RARRICON)));	
		// status bar
		//		GetClientRect(&wndPos);
		//		m_bar.SetPaneInfo(0,ID_INDICATOR_IP,SBPS_NORMAL, wndPos.right-50-iVidpan);
		m_statbar.GetWindowRect( &wndPos );
		ScreenToClient(&wndPos);
		wndPos.right -= iVidpan;
		m_statbar.MoveWindow(&wndPos);
		//
		showVideoPan=false;
	}
}

DWORD GetCurentIP(LPTSTR lpStringWithIP = 0)
{
    WSADATA wsaData;
	LPHOSTENT lphost;
    WSAStartup(MAKEWORD(1,1), &wsaData); // socket initialization
	
    char HostName[1024]; // host name buffer
    DWORD m_HostIP = 0;
	
    if(!gethostname(HostName, 1024)) // get host name
    {
        if(lphost = gethostbyname(HostName)) // get IP for the host name
            m_HostIP = ((LPIN_ADDR)lphost->h_addr)->s_addr; // convert from LPIN_ADDR to DWORD
    }
    WSACleanup(); // free the sockets
	
    if (lpStringWithIP) // если поинтер на строку не нулевой
		strcpy(lpStringWithIP, inet_ntoa(*((in_addr*)lphost->h_addr_list[0]))); // convert IP to string
	
    return m_HostIP;
}

CString ParseBytes(int ibutes)
{
	// add "b", "Kb", "Mb", "Gb" to the bytes
	CString sResult;
	if(ibutes< 1024)
		sResult.Format("%d b", ibutes);
	else if(ibutes/1024< 1024)
		sResult.Format("%.1f kb", (float)ibutes/1024.0);
	else if(ibutes/1024/1024< 1024)
		sResult.Format("%.1f Mb", (float)ibutes/1024.0/1024.0);
	else if(ibutes/1024/1024/1024< 1024)
		sResult.Format("%.1f Gb", (float)ibutes/1024.0/1024.0/1024.0);
	return sResult;
}

void CMyPhoneDlg::OnBmic() 
{
	// Mute Mic button pressed
	MuteMicrophoneCmd();	
}

void CMyPhoneDlg::OnBsnd() 
{
	// Mute Sound button pressed
	MuteSpeakerCmd();	
}

void CMyPhoneDlg::MuteMicrophoneCmd()
{
	H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
	if (connection == NULL)
		return;
	
	H323Channel * channel = connection->FindChannel(RTP_Session::DefaultAudioSessionID, FALSE);
	if (channel != NULL) {
		BOOL newState = !channel->IsPaused();
		channel->SetPause(newState);
		m_micMute.SetIcon( ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(newState?IDI_MICMUTEICON:IDI_MICICON))); 
	}
	
	connection->Unlock();
}

void CMyPhoneDlg::MuteSpeakerCmd()
{
	H323Connection * connection = m_endpoint.FindConnectionWithLock(m_token);
	if (connection == NULL)
		return;
	
	H323Channel * channel = connection->FindChannel(RTP_Session::DefaultAudioSessionID, TRUE);
	if (channel != NULL) {
		BOOL newState = !channel->IsPaused();
		channel->SetPause(newState);
		m_sndMute.SetIcon( ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(newState?IDI_SNDMUTEICON:IDI_SNDICON)));	
	}
	
	connection->Unlock();
}

// my windows messages handlers
LPARAM CMyPhoneDlg::OnFindGatekeeper(WPARAM wParam, LPARAM lParam)
{
	m_endpoint.FindGatekeeper();
	return 0;
}

void CMyPhoneDlg::TranslateDlg()
{	// changing window languge
	GetDlgItem(IDC_CONSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CONNECTIONSTR));
	GetDlgItem(IDC_CALLSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CALLTOSTR));
	GetDlgItem(IDC_CALL)->SetWindowText((LPCTSTR)LoadStringLang(IDS_CALLBSTR));
	GetDlgItem(IDC_ABOOK)->SetWindowText((LPCTSTR)LoadStringLang(IDS_ABOOKSTR));
	GetDlgItem(IDC_REFUSE)->SetWindowText((LPCTSTR)LoadStringLang(IDS_REFUSEBSTR));
	GetDlgItem(IDC_AUDIOSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_AUDIOSTR));	
	GetDlgItem(IDC_MSGSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_MSGSSTR));	
	GetDlgItem(IDC_VIDEOSTATIC)->SetWindowText((LPCTSTR)LoadStringLang(IDS_VIDEOSTR));	
	/*	// title
	CString strTitle;
	char strProgVersion[16];
	#ifdef MF_RELEASE
	sprintf(strProgVersion, "%d.%d", progMajorVersion, progMinorVersion);
	#else
	sprintf(strProgVersion, "%d.%db%d", progMajorVersion, progMinorVersion, progBuildVersion);
	#endif
	strTitle.Format((LPCTSTR)LoadStringLang(IDS_STRPROGNAMEVER), strProgName, strProgVersion);
	this->SetWindowText(strTitle);
	*/
	// menu
	CMenu* pMenu = GetMenu();
	if(pMenu)
	{
		pMenu->ModifyMenu( 0, MF_STRING | MF_BYPOSITION, ID_MSETTINGS, (LPCTSTR)LoadStringLang(IDS_MSETTINGSSTR));
		//		pMenu->ModifyMenu( 1, MF_STRING | MF_BYPOSITION, 1, (LPCTSTR)LoadStringLang(IDS_MABOUTSTR));
		CMenu* spMenu = pMenu->GetSubMenu(1);
		if(spMenu)
		{
			spMenu->ModifyMenu( 0, MF_STRING | MF_BYPOSITION, ID_MHELP, (LPCTSTR)LoadStringLang(IDS_MHELPSTR));
			spMenu->ModifyMenu( 2, MF_STRING | MF_BYPOSITION, ID_MABOUT, (LPCTSTR)LoadStringLang(IDS_ABOUTSTR));
		}
		//	    SetMainMenu();
	}
}

// Advanced Menu is abandoned, so this function is commented
/*
void CMyPhoneDlg::SetMainMenu(BOOL setH, int chSpace)
{
return;
if(!pMyMenu) return;
pMyMenu->RemoveMenu(1, MF_BYPOSITION );
DrawMenuBar();
UINT iMenuH = 0;
RECT wndPos, clPos;
GetWindowRect( &wndPos );
GetClientRect( &clPos );
UINT newSpace = clPos.right-clPos.left, oldSpace = pMyMenu->GetSpace();
if(chSpace==0)
{
MENUBARINFO mbInfo;
memset(&mbInfo, 0, sizeof(MENUBARINFO));
mbInfo.cbSize = sizeof(MENUBARINFO);
for(UINT i=0; i<=pMyMenu->GetMenuItemCount(); i++)
{
//		if(i == 2) continue;	// skip separator
if(GetMenuBarInfo(GetSafeHwnd(), OBJID_MENU, i, &mbInfo))
{
if(i == 0)
{
int t1 = mbInfo.rcBar.right;
int t2 = mbInfo.rcBar.left;
newSpace = mbInfo.rcBar.right-mbInfo.rcBar.left;
iMenuH = 2 + (mbInfo.rcBar.bottom-mbInfo.rcBar.top);
}
else
{
int t1 = mbInfo.rcBar.right;
int t2 = mbInfo.rcBar.left;
newSpace -= mbInfo.rcBar.right-mbInfo.rcBar.left;
}
}
}
//newSpace -= 20;
}
else
newSpace = oldSpace + chSpace;
if(newSpace)
pMyMenu->SetSpace(newSpace);
else
pMyMenu->SetSpace(oldSpace);
if(setH)
{
wndPos.bottom += iMenuH;
MoveWindow(&wndPos);
}
//
//	SetMenu(NULL);
//	SetMenu(pMyMenu);
pMyMenu->InsertMenu(1, MF_BYPOSITION | MF_OWNERDRAW | MF_SEPARATOR);
DrawMenuBar();
}
*/

#if PTRACING	// If Compiled with PTlib Tracing support (like in OpenH323)

BOOL CMyPhoneDlg::OpenTraceFile(PConfig & config)
{
	// setting TRACE options
	//PTrace::SetLevel(config.GetInteger(TraceLevelConfigKey, 1));
	PTrace::SetLevel(TraceLevel);
	PTrace::SetOptions(PTrace::FileAndLine);
	PTrace::SetOptions(PTrace::TraceLevel);
	PTrace::SetOptions(PTrace::DateAndTime);
	
	PString traceFileName = "myphone_trc.txt";
	
	// If already have a trace file, see if need to close it
	if (myTraceFile != NULL) {
		// If no change, do nothing more
		if (myTraceFile->GetFilePath() == PFilePath(traceFileName))
			return TRUE;
		
		PTrace::SetStream(NULL);
		delete myTraceFile;
		myTraceFile = NULL;
	}
	
	// Have stopped 
	if (traceFileName.IsEmpty())
		return TRUE;
	
	PTextFile * traceFile = new PTextFile;
	if (traceFile->Open(traceFileName, PFile::WriteOnly)) {
		myTraceFile = traceFile;
		PTrace::SetStream(traceFile);
		PProcess & process = PProcess::Current();
		PTRACE(0, process.GetName()
			<< " Version " << process.GetVersion(TRUE)
			<< " by " << process.GetManufacturer()
			<< " on " << process.GetOSClass() << ' ' << process.GetOSName()
			<< " (" << process.GetOSVersion() << '-' << process.GetOSHardware() << ')');
		
		return TRUE;
	}
	
	//  OutputStatusStr("ERROR! Trace failed.", S_SYSTEM);
	
	CString strMsg;
	strMsg.Format("MyPhone TRACE Failed!\nTrace output file name: %s", (const char *)traceFile->GetName());
	AfxMessageBox(strMsg);
	delete traceFile;
	return FALSE;
}

#endif

void CMyPhoneDlg::OnAbook() 
{
	// Call PhoneBook Dialog
	CMyPhoneBook abook;
	abook.DoModal();
}

CString CMyPhoneDlg::PhoneBookAddCall(int iType, CString &callName, CString &callAdr)
{
	// Add curent recepient to Outgoing, Incoming or Missed calls list
	//	if(!adrbook) return;
	CTime cutTime = CTime::GetCurrentTime();
	BOOL oldContact = false;
	PStringList allList = adrbook.GetSections();
	for(int ind=0; ind<allList.GetSize(); ind++)
	{
		if(adrbook.GetString(allList[ind], "Address", "-noaddress-")==(LPCTSTR)callAdr)
		{
			if((CString((LPCTSTR)allList[ind]).Left(8)=="-unknown") &&
				CopyContact(CString((LPCTSTR)allList[ind]), GetSimpleName("-new "+callName)))
			{
				adrbook.SetString(PString((LPCTSTR)GetSimpleName("-new "+callName)), "Name", PString((LPCTSTR)GetSimpleName("-new "+callName)));
				adrbook.DeleteSection(allList[ind]);
				allList[ind] = PString((LPCTSTR)GetSimpleName("-new "+callName));
			}
			switch(iType)
			{
			case 1:
				adrbook.SetString(allList[ind], "Outgoing", (LPCTSTR)cutTime.Format("%Y.%m.%d %H:%M:%S"));
				oldContact = true;
				break;
			case 2:
				adrbook.SetString(allList[ind], "Incoming", (LPCTSTR)cutTime.Format("%Y.%m.%d %H:%M:%S"));
				oldContact = true;
				break;
			case 3:
				adrbook.SetString(allList[ind], "Missed", (LPCTSTR)cutTime.Format("%Y.%m.%d %H:%M:%S"));
				oldContact = true;
				break;
			}
			return (LPCTSTR)adrbook.GetString(allList[ind], "Name", (LPCTSTR)callName);
			// break;
		}
	}
	//	if(!oldContact)
	//	{
	adrbook.SetString((LPCTSTR)("-unknown "+callName), "Name", (LPCTSTR)("-unknown "+callName));
	adrbook.SetString((LPCTSTR)("-unknown "+callName), "Address", (LPCTSTR)callAdr);
	adrbook.SetString((LPCTSTR)("-unknown "+callName), "RingSound", "-default-");
	switch(iType)
	{
	case 1:
		adrbook.SetString((LPCTSTR)("-unknown "+callName), "Outgoing", (LPCTSTR)cutTime.Format("%Y.%m.%d %H:%M:%S"));
		break;
	case 2:
		adrbook.SetString((LPCTSTR)("-unknown "+callName), "Incoming", (LPCTSTR)cutTime.Format("%Y.%m.%d %H:%M:%S"));
		break;
	case 3:
		adrbook.SetString((LPCTSTR)("-unknown "+callName), "Missed", (LPCTSTR)cutTime.Format("%Y.%m.%d %H:%M:%S"));
		break;
	}
	return callName;
	//	}
}


BOOL CMyPhoneDlg::CopyContact(CString &oldName, CString &newName)
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

CString CMyPhoneDlg::GetSimpleName(CString &fullName)
{
	// Try to Extract simple name from full h323 name
	int iAlias = fullName.Find('(');
	int iAdr = fullName.Find('[');
	if(iAlias>0)
		return fullName.Left(iAlias);
	else if(iAdr>0)
		return fullName.Left(iAdr);
	else
		return fullName;
}

CString CMyPhoneDlg::GetSimpleAdr(CString & fullAdr)
{
	// Try to extract IP from full h323 address
	int iFrom = fullAdr.Find('$');
	int iTo = fullAdr.Find(':');
	if(iFrom>=0)
		return fullAdr.Mid(iFrom+1, iTo>=0?iTo-iFrom-1:NULL);
	else
		return fullAdr;
	
}

CString CMyPhoneDlg::FindContactName(const H323Connection & connection)
{
	// Try to find contact in PhoneBook
	CString callName = (LPCTSTR)connection.GetRemotePartyName();
	CString callAdr = GetSimpleAdr(CString((LPCTSTR)connection.GetRemotePartyAddress()));
	PStringList allList = adrbook.GetSections();
	for(int ind=0; ind<allList.GetSize(); ind++)
		if(adrbook.GetString(allList[ind], "Address", "-noaddress-")==(LPCTSTR)callAdr)
			return (LPCTSTR)adrbook.GetString(allList[ind], "Name", (LPCTSTR)callName);
		return callName;
}

////////////////////////////////////////////////////////////////////////////
// Tray Icon related Methods

void CMyPhoneDlg::AddIcon()
{
    // Add icon to system tray through Shell_NotifyIcon API
    NOTIFYICONDATA IconData;
    IconData.cbSize = sizeof(NOTIFYICONDATA);
    IconData.uID    = IDI_PHONE16;
    IconData.hWnd   = this->GetSafeHwnd();
    IconData.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
    IconData.uCallbackMessage = WM_TRAYNOTIFY;
    strcpy(IconData.szTip, strProgName);
    IconData.hIcon  = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PHONE16));
	
    Shell_NotifyIcon(NIM_ADD,&IconData);
}

void CMyPhoneDlg::RemoveIcon()
{
	// Remove icon from try
    NOTIFYICONDATA IconData;
    IconData.cbSize = sizeof(NOTIFYICONDATA);
    IconData.uID    = IDI_PHONE16;
    IconData.hWnd   = this->GetSafeHwnd();
	
    Shell_NotifyIcon(NIM_DELETE,&IconData);
}

void CMyPhoneDlg::ChangeIcon(CString &strMsg, int iType)
{
	// Change Icon and Baloon Tooltip(if supported) 
	// Do not do enything if the window is not minimized
	WINDOWPLACEMENT wplsmt;
	GetWindowPlacement(&wplsmt);
	if (!(wplsmt.showCmd & SW_MINIMIZE))
		return;
	// Use my MYNOTIFYICONDATA insted of standard NOTIFYICONDATA structure 
	// becouse I do not have new headers of  M$ SDK so I just redefined the structure
	// manualy just like it should be in oversion 5.00 and later..
    MYNOTIFYICONDATA IconData;	
    IconData.cbSize = sizeof(MYNOTIFYICONDATA);
    IconData.uID    = IDI_PHONE16;
    IconData.hWnd   = this->GetSafeHwnd();
    IconData.uFlags = strMsg.IsEmpty()?NIF_ICON:NIF_ICON | NIF_INFO;
	if(!strMsg.IsEmpty())
	{
		strcpy(IconData.szInfo, (LPCTSTR)strMsg);
		strcpy(IconData.szInfoTitle, strProgName);
		IconData.dwInfoFlags=NIIF_INFO;
		IconData.uTimeout = 10000; // in milliseconds
	}
    if((!TrayAct && iType==0) || iType==1)
		IconData.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PHONE16));
    if((TrayAct && iType==0) || iType==2)
		IconData.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PHONE16UP));
	TrayAct=!TrayAct;
	
    Shell_NotifyIcon(NIM_MODIFY,(PNOTIFYICONDATA) &IconData);
}

LPARAM CMyPhoneDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	// Processing Notify messages from tray icon
    switch(lParam)
    {
	case WM_RBUTTONUP:
		
		//POINT WinPoint;
		//GetCursorPos(&WinPoint);
		//SetForegroundWindow(this->GetSafeHdc());
		//PopupMenu1->Popup(WinPoint.x, WinPoint.y);
		//PostMessage(Handle, WM_NULL, 0,0);
		//MessageBox("Menu");
		break;
	case WM_LBUTTONUP:
		WINDOWPLACEMENT wplsmt;
		GetWindowPlacement(&wplsmt);
		if (/*!IsWindowVisible()*/ wplsmt.showCmd & SW_MINIMIZE) //if minimized
		{
			PostMessage(WM_SYSCOMMAND, SC_RESTORE);
			SetForegroundWindow();
			ChangeIcon(CString(""), 1);
		}
		else
		{
			PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		}
		break;
    }
	
	return 0;
}

void CMyPhoneDlg::moveControl(int ctrl, BOOL add, int iType)
{
	RECT wndRect;
	CWnd * wndCtrl = GetDlgItem(ctrl);
	if(!wndCtrl) return;
	wndCtrl->GetWindowRect(&wndRect);
	ScreenToClient(&wndRect);
	switch(iType)
	{
	case 0:		// Resize window 
		wndRect.right = add?wndRect.right+176:wndRect.right-176;
		wndRect.bottom = add?wndRect.bottom+144:wndRect.bottom-144;
		break;
	case 1:		// Resize window horizontally
		wndRect.right = add?wndRect.right+176:wndRect.right-176;
		break;
	case 2:		// Resize window vertically
		wndRect.bottom = add?wndRect.bottom+144:wndRect.bottom-144;
		break;
	case 10:		// Move window
		wndRect.right = add?wndRect.right+176:wndRect.right-176;
		wndRect.bottom = add?wndRect.bottom+144:wndRect.bottom-144;
		wndRect.left = add?wndRect.left+176:wndRect.left-176;
		wndRect.top = add?wndRect.top+144:wndRect.top-144;
		break;
	case 11:		// Move window horizontally
		wndRect.right = add?wndRect.right+176:wndRect.right-176;
		wndRect.left = add?wndRect.left+176:wndRect.left-176;
		break;
	case 12:		// Move window vertically
		wndRect.bottom = add?wndRect.bottom+144:wndRect.bottom-144;
		wndRect.top = add?wndRect.top+144:wndRect.top-144;
		break;
	}
	wndCtrl->MoveWindow(&wndRect);
}

// Change video panels size
void CMyPhoneDlg::changeVideoSize(VIDEOSIZE vSize)
{
	// Enable/Disable hidePnP button
	if(m_endpoint.localVideo && vSize == CIF)
	{
		m_hidePnPCtrl.ShowWindow(SW_SHOW);
	}
	else
	{
		m_hidePnPCtrl.ShowWindow(SW_HIDE);
		m_hidePnPCtrl.SetCheck(FALSE);		// Reset the button state
		hidePnP = FALSE;
		m_hidePnPCtrl.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PNPSHOW)));
	}
	//
	if(curVideoSize == vSize) return;
	RECT wndPos;
	switch(vSize)
	{
	case QCIF:
		GetDlgItem(IDC_VIDEOBITMAP1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SLVIDEO)->ShowWindow(SW_SHOW);
		m_hidePnPCtrl.ShowWindow(SW_HIDE);
		//
		moveControl(IDC_VIDEOBITMAP, FALSE, 0);
		moveControl(IDC_VIDEOSTATIC, FALSE, 1);
		moveControl(IDC_SRVIDEO, FALSE, 1);
		if(showVideoPan)
		{
		  GetWindowRect( &wndPos );
		  wndPos.right -= 176;
		  MoveWindow(&wndPos);
		  moveControl(IDC_SMYSTAT, FALSE, 1);
		  moveControl(IDC_BVIDPAN, FALSE, 11);
		}
		//
		curVideoSize = vSize;
		break;

	case CIF:
		GetDlgItem(IDC_VIDEOBITMAP1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SLVIDEO)->ShowWindow(SW_HIDE);
		//
		if(showVideoPan)
		{
		  GetWindowRect( &wndPos );
		  wndPos.right += 176;
		  MoveWindow(&wndPos);
		  moveControl(IDC_SMYSTAT, TRUE, 1);
		  moveControl(IDC_BVIDPAN, TRUE, 11);
		}
		moveControl(IDC_SRVIDEO, TRUE, 1);
		moveControl(IDC_VIDEOSTATIC, TRUE, 1);
		moveControl(IDC_VIDEOBITMAP, TRUE, 0);
		//
		curVideoSize = vSize;
		break;
	}
}

void CMyPhoneDlg::OnHidepnpcheck() 
{
	hidePnP = m_hidePnPCtrl.GetCheck();	
	m_hidePnPCtrl.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(hidePnP?IDI_PNPHIDE:IDI_PNPSHOW)));
}

/////////////////////////////////////////////////////////////////////////////
// CVideoDlg dialog used for App About

CVideoDlg::CVideoDlg() : CDialog(CVideoDlg::IDD)
{
 d3d_mode = 0;
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoDlg, CDialog)
ON_WM_SYSCOMMAND()
ON_WM_LBUTTONDBLCLK()
ON_WM_KEYDOWN()
ON_WM_SYSKEYDOWN()
ON_WM_CLOSE()
ON_WM_DESTROY()
ON_WM_SHOWWINDOW()
//ON_WM_PAINT()
//ON_WM_QUERYDRAGICON()
//ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CVideoDlg::OnInitDialog() 
{
	return CDialog::OnInitDialog();
	
//	return TRUE;  // return TRUE unless you set the focus to a control
}


void CVideoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	PTRACE(2, "MyPhone\tOnSysCommand\t" << nID << " " << lParam);

        if(nID == SC_CLOSE)  // Closing from pressing [X]
        {
        }
	else if(nID == SC_MINIMIZE)
	{
//		CDialog::OnSysCommand(nID, lParam);
//		ShowWindow(SW_HIDE);
	}
	else if(nID == SC_MAXIMIZE)
	{
		ShowWindow(SW_SHOW);
		CDialog::OnSysCommand(nID, lParam);
	}
        else
	{
         CDialog::OnSysCommand(nID, lParam);
	}
}

void CVideoDlg::OnLButtonDblClk(UINT nID, CPoint point)
{
 d3d_mode^=1;
}

void CVideoDlg::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	PTRACE(2, "MyPhone\tOnKeyDown\t" << nChar);
	CDialog::OnKeyDown(nChar,nRepCnt,nFlags);
}

void CVideoDlg::OnSysKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	PTRACE(2, "MyPhone\tOnSysKeyDown\t" << nChar);
	CDialog::OnKeyDown(nChar,nRepCnt,nFlags);
}

void CVideoDlg::OnClose( )
{
 PTRACE(2, "MyPhone\tOnClose");
 CDialog::OnClose();
}

void CVideoDlg::OnDestroy()
{
 PTRACE(2, "MyPhone\tOnDestroy");
 CDialog::OnDestroy();
}


void CVideoDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
 PTRACE(2, "MyPhone\tOnShowWindow " << bShow << " " << nStatus);
 CDialog::OnShowWindow(bShow,nStatus);
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN)
        {
		  return TRUE;
        }
        if(pMsg->message==WM_SYSKEYDOWN)
        {
                  return TRUE;
        }
	return CDialog::PreTranslateMessage(pMsg);
}

