///////////////////////////////////////////////////////////////////////////////
// MyPhoneEndPoint.cpp: implementation for the CMyPhoneEndPoint class.
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
#include "MyPhoneEndPoint.h"
#include "MyPhoneDlg.h"
#include "CVideoDevice.h"

#include "Multilang.h"		// Languages

#ifdef _WIN32
#define strdup _strdup
#define itoa _itoa_s
#endif

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//#if defined(P_DSHOW) /*|| defined(P_DIRECTSHOW)*/
  #pragma message("-->DirectShow Enabled")
//  #include <ptlib\dshow.h>
  PString CMyPhoneEndPoint::VideoInputDriver = "DirectShow";
//#else
//  PString CMyPhoneEndPoint::VideoInputDriver = "VideoForWindows";
//#endif

/////////////////////////// Config Keys
const char Ready[] = "Ready. IP address is ";

const char DtmfAsStringConfigKey[] = "DtmfAsString";
const char BandwidthConfigKey[] = "Bandwidth";
const char ListenerInterfaceConfigKey[] = "ListenerInterface";
const char RouterConfigKey[] = "NATRouterAddress";

const char RTPPortBaseConfigKey[] = "RTPPortBase";
const char RTPPortMaxConfigKey[] = "RTPPortMax";

const char NoTunnelingConfigKey[] = "NoTunneling";
const char NoFastStartConfigKey[] = "FastStart";
const char StrictSingleLineConfigKey[] = "SingleLine";
const char IpTosConfigKey[] = "IpTOS";
const char UserInputModeConfigKey[] = "UserInputMode";
const char UsernameConfigKey[] = "Username";

const char AliasConfigKey[] = "UserAliases";

const char AutoAnswerConfigKey[] = "AutoAnswer";
const char AutoMuteConfigKey[] = "AutoMute";

const char CodecsConfigSection[] = "Codecs";
const char OnCodecSuffix[] = " (On)";
const char OffCodecSuffix[] = " (Off)";

const char DiscoverGatekeeperConfigKey[] = "DiscoverGatekeeper";
const char UseGatekeeperConfigKey[] = "UseGatekeeper";
const char RequireGatekeeperConfigKey[] = "RequireGatekeeper";
const char TimeToLiveConfigKey[] = "TimeToLive";
const char GatekeeperHostConfigKey[] = "GatekeeperHost";
const char GatekeeperIdConfigKey[] = "GatekeeperId";
const char GatekeeperPassConfigKey[] = "GatekeeperPassword";

const char SoundPlayConfigKey[] = "SoundPlayDevice";
const char SoundRecordConfigKey[] = "SoundRecordDevice";
const char SilenceDetectConfigKey[] = "SilenceDetect";
const char AECEnableConfigKey[] = "AECEnable";
const char AGCEnableConfigKey[] = "AGCEnable";
const char JitterConfigKey[] = "Jitter";
const char BufferCountConfigKey[] = "BufferCount";

const char AutoTransmitVideoConfigKey[] = "AutoTransmitVideo";
const char AutoReceiveVideoConfigKey[] = "AutoReceiveVideo";
const char VideoCodecsConfigSection[] = "VideoCodecs";
const char VideoDeviceConfigKey[] = "VideoDevice";
const char VideoInSizeConfigKey[] = "VideoInSize";
const char VideoOutSizeConfigKey[] = "VideoOutSize";
const char VideoSourceConfigKey[] = "VideoSource";
const char VideoFormatConfigKey[] = "VideoFormat";
const char VideoLocalConfigKey[] = "VideoLocal";
const char VideoFlipLocalConfigKey[] = "VideoFlipLocal";
const char VideoQualityConfigKey[] = "VideoQuality";
const char VideoInMaxbandWidthKey[] = "VideoInMaxbandwidth";
const char VideoOutMaxbandWidthKey[] = "VideoOutMaxbandwidth";
const char VideoFPSKey[] = "VideoFramesPerSecond";
const char VideoPacketSizeConfigKey[] = "VideoPacketSize";
const char VideoInVFlipConfigKey[] = "VideoInVFlip";
const char VideoInHFlipConfigKey[] = "VideoInHFlip";
const char VideoOutVFlipConfigKey[] = "VideoOutVFlip";
const char VideoOutHFlipConfigKey[] = "VideoOutHFlip";

//////////////////////////////////////////////////////////////////////
// Derived Connection
CMyPhoneConnection::CMyPhoneConnection(
		   CMyPhoneDlg* wnd, CMyPhoneEndPoint & ep, unsigned callReference, unsigned options)
		   : H323Connection(ep, callReference, options), endpoint(ep), m_dialog(wnd)
{
}

BOOL CMyPhoneConnection::OnAlerting(const H323SignalPDU &, const PString & user)
{
	CString fixuser = m_dialog->FindContactName(*this);
	m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_RINGSTR), S_SYSTEM, (const char *)fixuser);
	CString allStr;
	allStr.Format((LPCTSTR)LoadStringLang(IDS_RING1STR), (const char*) fixuser);
	m_dialog->m_caller.SetWindowText((LPCTSTR)allStr);
	return TRUE;
}

/*
BOOL CMyPhoneConnection::OpenLogicalChannel(const H323Capability & capability,
                                        unsigned sessionID,
                                        H323Channel::Directions dir)
{
 PString name = capability.GetFormatName();

 return H323Connection::OpenLogicalChannel(capability, sessionID, dir);
}
*/

void CMyPhoneConnection::OnUserInputString(const PString & value)
{
	m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_REPLMSGSTR), S_INMSG, 
		""/*m_dialog->FindContactName(*this)*//*(const char *) this->GetRemotePartyName()*/, 
		(const char *) value);
} 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PluginLoaderStartup2  CMyPhoneApp::PWLibProcess::pluginLoader;
H323PluginCodecManager * CMyPhoneApp::PWLibProcess::plugmgr=NULL;


CMyPhoneEndPoint::CMyPhoneEndPoint()
{
	m_dialog = NULL;
	m_vdlg = NULL;
	m_stat.iSecs=0;  m_stat.ibSent=0;  m_stat.ibRcvd=0;  m_stat.iDelay=0;
//	videoSize = 0;
	m_router = NULL;
	localVideo = FALSE;

	// MFC applications are not at all plugin friendly
	// You need to manually add the plugins
	CMyPhoneApp::PWLibProcess::LoadPluginMgr();
	CMyPhoneApp::PWLibProcess::pluginLoader.OnStartup();
}

CMyPhoneEndPoint::~CMyPhoneEndPoint()
{
	// You need to manually remove the plugins
	CMyPhoneApp::PWLibProcess::RemovePluginMgr();
	CMyPhoneApp::PWLibProcess::pluginLoader.OnShutdown();
}

BOOL CMyPhoneEndPoint::Initialise(CMyPhoneDlg *dlg, CVideoDlg *vdlg)
{
	m_dialog = dlg;
        m_vdlg = vdlg;
	isIncomingCall = FALSE;

	SetAudioJitterDelay(50, config.GetInteger(JitterConfigKey, GetMaxAudioJitterDelay()));
	SetSoundChannelBufferDepth(config.GetInteger(BufferCountConfigKey, GetSoundChannelBufferDepth()));
	
	// UserInput mode
    // Backward compatibility configuration entry
    unsigned mode = H323Connection::SendUserInputAsString;
	m_fDtmfAsString=true;
    if (config.HasKey(DtmfAsStringConfigKey)) {
		if (!config.GetBoolean(DtmfAsStringConfigKey))
		{
			mode = H323Connection::SendUserInputAsTone;
			m_fDtmfAsString=false;
		}
		config.DeleteKey(DtmfAsStringConfigKey);
	}
	mode = config.GetInteger(UserInputModeConfigKey, mode);
	SetSendUserInputMode((H323Connection::SendUserInputModes)mode);

//        SetSoundChannelPlayDriver("WindowsMutimedia");
//        SetSoundChannelRecordDriver("WindowsMultimedia");
//        SetSoundChannelPlayDriver("DirectSound");
//        SetSoundChannelRecordDriver("DirectSound");
	
        PString soundPlayStr = config.GetString(SoundPlayConfigKey, GetSoundChannelPlayDevice());
        PStringArray soundPlayer = soundPlayStr.Tokenise('\t');
        BOOL fPlayerAvailable;
        if(soundPlayer.GetSize()==1)
        {  
         SetSoundChannelPlayDriver("WindowsMutimedia");
	 fPlayerAvailable = SetSoundChannelPlayDevice(soundPlayer[0]);
         PTRACE(1, "MyPhone\tSetSoundPlayer Driver:WindowsMultimedia, Device:" << soundPlayer[0]);
        }
        else
        {
         SetSoundChannelPlayDriver(soundPlayer[0]);
	 fPlayerAvailable = SetSoundChannelPlayDevice(soundPlayer[1]);
         PTRACE(1, "MyPhone\tSetSoundPlayer Driver:" << soundPlayer[0] << ", Device:" << soundPlayer[1]);
	}

        PString soundRecordStr = config.GetString(SoundRecordConfigKey, GetSoundChannelRecordDevice());
        PStringArray soundRecorder = soundRecordStr.Tokenise('\t');
        BOOL fRecorderAvailable;
        if(soundRecorder.GetSize()==1)
        {  
         SetSoundChannelRecordDriver("WindowsMutimedia");
	 fRecorderAvailable = SetSoundChannelRecordDevice(soundRecorder[0]);
         PTRACE(1, "MyPhone\tSetSoundRecorder Driver:WindowsMultimedia, Device:" << soundRecorder[0]);
        }
        else
        {
         SetSoundChannelRecordDriver(soundRecorder[0]);
	 fRecorderAvailable = SetSoundChannelRecordDevice(soundRecorder[1]);
         PTRACE(1, "MyPhone\tSetSoundRecorder Driver:" << soundRecorder[0] << ", Device:" << soundRecorder[1]);
	}

	m_fAECOn = config.GetBoolean(AECEnableConfigKey, FALSE);
	m_fAGCOn = config.GetInteger(AGCEnableConfigKey, 2000);
	SetAECAlgo(m_fAECOn);
	agc = m_fAGCOn;
	             
	// set some oter settings from Config
	m_fNoFastStart =  config.GetBoolean(NoFastStartConfigKey, FALSE);
    DisableFastStart(m_fNoFastStart);
	m_fStrictSingleLine =  config.GetBoolean(StrictSingleLineConfigKey, FALSE);
    StrictSingleLine(m_fStrictSingleLine);

	m_fDoH245Tunnelling = !(config.GetBoolean(NoTunnelingConfigKey, FALSE));
    DisableH245Tunneling(!m_fDoH245Tunnelling);
	m_fSilenceOn = config.GetBoolean(SilenceDetectConfigKey, TRUE);
    SetSilenceDetectionMode(m_fSilenceOn
		? H323AudioCodec::AdaptiveSilenceDetection
		: H323AudioCodec::NoSilenceDetection);
    SetLocalUserName(config.GetString(UsernameConfigKey, GetLocalUserName()));
    SetInitialBandwidth((unsigned)(config.GetReal(BandwidthConfigKey, 10000)*20));
    SetRtpIpTypeofService(config.GetInteger(IpTosConfigKey, GetRtpIpTypeofService()));
	
	SetRtpIpPorts(config.GetInteger(RTPPortBaseConfigKey, GetRtpIpPortBase()),
		config.GetInteger(RTPPortMaxConfigKey,  GetRtpIpPortBase()));
	if(config.HasKey(RouterConfigKey))
		m_router = config.GetString(RouterConfigKey, m_router.AsString());
	
	m_fAutoAnswer = config.GetBoolean(AutoAnswerConfigKey, false);
	m_fAutoMute = config.GetBoolean(AutoMuteConfigKey, false);
	
    CString alias, aliases;
	aliases = CString((const char *)config.GetString(AliasConfigKey, _T("")));
	aliases.TrimLeft();
	int iPos=0;
    while ((iPos = aliases.Find(_T("|")))>0)  // loading user aliases
	{
		alias = aliases.Left(iPos);
		aliases.Delete(0,iPos+1);
		AddAliasName((LPCTSTR)alias);
	}
	
	// The order in which capabilities are added to the capability table
	// determines which one is selected by default.
	LoadCapabilities();
	
	PString interfaces = config.GetString(ListenerInterfaceConfigKey, "*");
	if(StartListeners(interfaces.Tokenise(',')))
	{
		m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_MPREADY1STR), S_SYSTEM, (const char *)strProgName);
		return true;
	}
	else
	{
		m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRLSNSTR), S_SYSTEM, (const char *)strProgName, (const char *)interfaces);
		return false;
	}
}

H323Connection * CMyPhoneEndPoint::CreateConnection(unsigned int refID)
{
	// close video panel before everything else
	if(m_dialog->autohideVideoPan && m_dialog->showVideoPan )
		m_dialog->ShowVideoPanels(FALSE);  // hide Video panel if it's visible
	
	return new CMyPhoneConnection(m_dialog, *this, refID, 0);
}

BOOL CMyPhoneEndPoint::OnIncomingCall(H323Connection & connection,
									  const H323SignalPDU &,
									  H323SignalPDU &)
{
	CTime tNow;
	tNow=CTime::GetCurrentTime();
	isIncomingCall=TRUE;
	return TRUE;
}

void CMyPhoneEndPoint::OnConnectionEstablished(H323Connection & connection, const PString & token)
{
	m_dialog->m_token = token;
	CString goodName = (LPCTSTR)connection.GetRemotePartyName();
	// store last number in combobox
	if(!connection.HadAnsweredCall())
	{
		m_dialog->PhoneBookAddCall(1, CString((LPCTSTR)connection.GetRemotePartyName()), m_dialog->GetSimpleAdr(CString((LPCTSTR)connection.GetRemotePartyAddress())));
		m_dialog->StoreDestonation(goodName /*(const char *)connection.GetRemotePartyName()*/);
	}
	else
	{
		// store in Address Book
		m_dialog->PhoneBookAddCall(2, CString((LPCTSTR)connection.GetRemotePartyName()), m_dialog->GetSimpleAdr(CString((LPCTSTR)connection.GetRemotePartyAddress())));
		if(m_dialog->autoAddInAddr)
			m_dialog->StoreDestonation(goodName /*(const char *)connection.GetRemotePartyName()*/, TRUE);
	}
	m_dialog->OnConnectionEstablished(m_dialog->FindContactName(connection) /*(const char *)connection.GetRemotePartyName()*/);
}

void CMyPhoneEndPoint::OnConnectionCleared(H323Connection & connection, const PString & clearedCallToken)
{
	if(connection.GetCallEndReason()==H323Connection::EndedByRemoteBusy)
		m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_CALLBUSYSTR), S_SYSTEM, m_dialog->FindContactName(connection));

	// clearing statistic
	m_stat.iSecs=0;
	m_stat.ibSent=0;
	m_stat.ibRcvd=0;
	m_stat.iDelay=0;
	// UI change
	m_dialog->OnConnectionCleared(m_dialog->FindContactName(connection));
}

H323Connection::AnswerCallResponse CMyPhoneEndPoint::OnAnswerCall(H323Connection & connection, const PString &caller, const H323SignalPDU &, H323SignalPDU &)
{
	return m_dialog->OnAnswerCall(connection) ? H323Connection::AnswerCallNow : H323Connection::AnswerCallPending;
}

BOOL CMyPhoneEndPoint::OpenAudioChannel(H323Connection & connection,
										BOOL isEncoding,
										unsigned bufferSize,
										H323AudioCodec & codec)
{
	return H323EndPoint::OpenAudioChannel(connection, isEncoding, bufferSize, codec);
}

BOOL CMyPhoneEndPoint::OnStartLogicalChannel(H323Connection &, H323Channel & channel)
{
	if( isIncomingCall
          && m_fAutoMute
          && (channel.GetCapability().GetMainType()==H323Capability::e_Audio)
          && (channel.GetDirection()==H323Channel::IsTransmitter)
        )
        {
          channel.SetPause(TRUE);
	  m_dialog->m_micMute.SetCheck(BST_CHECKED);
          m_dialog->m_micMute.SetIcon(::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_MICMUTEICON)));
        }

	OnLogicalChannel(channel, IDS_STARTCHANNELTX, IDS_STARTCHANNELRX);
	return TRUE;
}

void CMyPhoneEndPoint::OnLogicalChannel(const H323Channel & channel, unsigned txStrID, unsigned rxStrID)
{
	const H323Capability & capability = channel.GetCapability();
	PString name = capability.GetFormatName();
	PString frames;

	if (capability.GetMainType() == H323Capability::e_Video)
	{
		unsigned numFrames = channel.GetDirection() == H323Channel::IsTransmitter
			? capability.GetTxFramesInPacket()
			: capability.GetRxFramesInPacket();
		frames.sprintf(" (%u frames)", numFrames);
	}
//	else if (capability.GetMainType() == H323Capability::e_Video)
//	{
//		frames.sprintf(" (%ux%u pixels)", videoWidth, videoHeight);
//	}
	
	switch (channel.GetDirection())
	{
    case H323Channel::IsTransmitter :
		m_dialog->OutputStatus(txStrID, (const char *)name, (const char *)frames);
		break;
		
    case H323Channel::IsReceiver :
		m_dialog->OutputStatus(rxStrID, (const char *)name, (const char *)frames);
		break;
		
    default :
		break;
	}
}

void CMyPhoneEndPoint::OnClosedLogicalChannel(H323Connection &, const H323Channel & channel)
{
	OnLogicalChannel(channel, IDS_STOPCHANNELTX, IDS_STOPCHANNELRX);
}

void CMyPhoneEndPoint::LoadCapabilities()
{

 BOOL sizeChange = FALSE;

 capabilities.RemoveAll();
	
// Add the codecs we know about
 AddAllCapabilities(0, 0, "*"); 

// Удаляю не поддерживаемые видео кодеки из реестра
 PINDEX videoNum = 0;
 PStringArray duplicateCheck;
 for (;;)
 {
  PString key = psprintf("%u", ++videoNum);
  PString name = config.GetString(VideoCodecsConfigSection, key, "");
  if (name.IsEmpty()) break;

  PINDEX suffixPos = name.Find(OnCodecSuffix);
  if (suffixPos != P_MAX_INDEX) 
   name.Delete(suffixPos, P_MAX_INDEX);
  else 
  {
   suffixPos = name.Find(OffCodecSuffix);
   name.Delete(suffixPos, P_MAX_INDEX);
  }

  int res = 0;
  if(duplicateCheck.GetStringsIndex(name) == P_MAX_INDEX)
  for (PINDEX i = 0; i < capabilities.GetSize(); i++)
  {
   if (capabilities[i].GetMainType() == H323Capability::e_Video)
   {
    if(capabilities[i].GetFormatName() == name)
     { res = 1; duplicateCheck.AppendString(name); break; }
   }
  }
  if(res == 0) 
  {
   PINDEX j = videoNum; videoNum--;
   for (;;)
   {
    PString key1 = psprintf("%u", ++j);
    PString name1 = config.GetString(VideoCodecsConfigSection, key1, "");
    if (name1.IsEmpty()) break;

    config.SetString(VideoCodecsConfigSection, psprintf("%u", j-1), name1);
   }
   config.DeleteKey(VideoCodecsConfigSection, psprintf("%u", j-1));
  }
 }

// добавляю новые видео кодеки если их нет в конфигурации
 for (PINDEX i = 0; i < capabilities.GetSize(); i++)
 {
  if (capabilities[i].GetMainType() == H323Capability::e_Video)
  {
   PINDEX codecNum=0;
   int res = 0;
   int suffix = 0;
   for (;;)
   {
    PString key = psprintf("%u", ++codecNum);
    PString name = config.GetString(VideoCodecsConfigSection, key, "");
    if (name.IsEmpty()) break;   

    suffix = 0;
    PINDEX suffixPos = name.Find(OnCodecSuffix);
    if (suffixPos != P_MAX_INDEX)
     name.Delete(suffixPos, P_MAX_INDEX);
    else
    {
     suffix = 1;
     suffixPos = name.Find(OffCodecSuffix);
     name.Delete(suffixPos, P_MAX_INDEX);
    }
  
    if(capabilities[i].GetFormatName() == name) { res = 1; break; }
   }
   if(res == 0)
   {
    config.SetString(VideoCodecsConfigSection,
        psprintf("%u", codecNum),
        capabilities[i].GetFormatName() + ((suffix==0)?OnCodecSuffix:OffCodecSuffix));
   }
  }
 }

 PINDEX audioNum = 0;
 PStringArray audioDupCheck;
 for (;;)
 {
  PString key = psprintf("%u", ++audioNum);
  PString name = config.GetString(CodecsConfigSection, key, "");
  if (name.IsEmpty()) break;

  PINDEX suffixPos = name.Find(OnCodecSuffix);
  if (suffixPos != P_MAX_INDEX) 
   name.Delete(suffixPos, P_MAX_INDEX);
  else 
  {
   suffixPos = name.Find(OffCodecSuffix);
   name.Delete(suffixPos, P_MAX_INDEX);
  }

  int res = 0;
  if(audioDupCheck.GetStringsIndex(name) == P_MAX_INDEX)
  for (PINDEX i = 0; i < capabilities.GetSize(); i++)
  {
   if (capabilities[i].GetMainType() == H323Capability::e_Audio)
   {
    if(capabilities[i].GetFormatName() == name)
     { res = 1; audioDupCheck.AppendString(name); break; }
   }
  }
  if(res == 0) 
  {
   PINDEX j = audioNum; audioNum--;
   for (;;)
   {
    PString key1 = psprintf("%u", ++j);
    PString name1 = config.GetString(CodecsConfigSection, key1, "");
    if (name1.IsEmpty()) break;

    config.SetString(CodecsConfigSection, psprintf("%u", j-1), name1);
   }
   config.DeleteKey(CodecsConfigSection, psprintf("%u", j-1));
  }
 }

 for (PINDEX i = 0; i < capabilities.GetSize(); i++)
 {
  if (capabilities[i].GetMainType() == H323Capability::e_Audio)
  {
   PINDEX codecNum=0;
   int res = 0;
   int suffix = 0;
   for (;;)
   {
    PString key = psprintf("%u", ++codecNum);
    PString name = config.GetString(CodecsConfigSection, key, "");
    if (name.IsEmpty()) break;   

    suffix = 0;
    PINDEX suffixPos = name.Find(OnCodecSuffix);
    if (suffixPos != P_MAX_INDEX)
     name.Delete(suffixPos, P_MAX_INDEX);
    else
    {
     suffix = 1;
     suffixPos = name.Find(OffCodecSuffix);
     name.Delete(suffixPos, P_MAX_INDEX);
    }
  
    if(capabilities[i].GetFormatName() == name) { res = 1; break; }
   }
   if(res == 0)
   {
    config.SetString(CodecsConfigSection,
        psprintf("%u", codecNum),
        capabilities[i].GetFormatName() + ((suffix==0)?OnCodecSuffix:OffCodecSuffix));
   }
  }
 }

// Add all the UserInput capabilities
 AddAllUserInputCapabilities(0, 1);

 int videoSizeRx = config.GetInteger(VideoInSizeConfigKey, 2);
 int videoSizeTx = config.GetInteger(VideoOutSizeConfigKey, 2);

 RemoveCapability(H323Capability::e_ExtendVideo);

 autoStartTransmitVideo = config.GetBoolean(AutoTransmitVideoConfigKey, TRUE);
 autoStartReceiveVideo = config.GetBoolean(AutoReceiveVideoConfigKey, TRUE);
	
 localVideo = config.GetBoolean(VideoLocalConfigKey, FALSE);
 localFlip = config.GetBoolean(VideoFlipLocalConfigKey, FALSE);
	
 int videoInMaxBitRate = config.GetInteger(VideoInMaxbandWidthKey, 320);
 videoInMaxBitRate = 1024 * PMAX(16, PMIN(10240, videoInMaxBitRate));

// changing audio codecs
 PStringArray enabledCodecs;
 PINDEX codecNum = 0;
 for (;;) 
 {
  PString key = psprintf("%u", ++codecNum);
  PString name = config.GetString(CodecsConfigSection, key, "");
  if (name.IsEmpty()) break;
		
  PINDEX suffixPos = name.Find(OffCodecSuffix);
  if (suffixPos != P_MAX_INDEX) 
  {
   capabilities.Remove(name(0, suffixPos-1));
   continue;
  }
		
  suffixPos = name.Find(OnCodecSuffix);
  if (suffixPos != P_MAX_INDEX)	name.Delete(suffixPos, P_MAX_INDEX);
  enabledCodecs.AppendString(name);
 }

 codecNum = 0;
 for (;;) 
 {
  PString key = psprintf("%u", ++codecNum);
  PString name = config.GetString(VideoCodecsConfigSection, key, "");
  if (name.IsEmpty()) break;
 }

 int tvNum = 0;
// if(tvCaps==NULL)  this generates error in free
 tvCaps = (char **)calloc(codecNum+1,sizeof(char *));
// while(tvCaps[tvNum]!=NULL) { free(tvCaps[tvNum]); tvCaps[tvNum]=NULL; tvNum++; }

 tvNum = 0;
 codecNum = 0;
 for (;;) 
 {
  PString key = psprintf("%u", ++codecNum);
  PString name = config.GetString(VideoCodecsConfigSection, key, "");
  if (name.IsEmpty()) break;

// удаление отключенных кодеков		
  PINDEX suffixPos = name.Find(OffCodecSuffix);
  if (suffixPos != P_MAX_INDEX) 
  {
   capabilities.Remove(name(0, suffixPos-1)); continue;
  }
// удаление суффикса on из имени кодека
  suffixPos = name.Find(OnCodecSuffix);
  if (suffixPos != P_MAX_INDEX)	name.Delete(suffixPos, P_MAX_INDEX);

// проверка кодека на соответствие размеру принимаемой картинки
// (меньше можно, больше нельзя) и удаление из списка локальных кодеков
  suffixPos = P_MAX_INDEX;
  switch(videoSizeRx)
  {
   case 0: //QCIF
    suffixPos = name.Find("-CIF");   if(suffixPos != P_MAX_INDEX) break;
   case 1: //QVGA
    suffixPos = name.Find("-360P");  if(suffixPos != P_MAX_INDEX) break;
   case 2: //CIF
    suffixPos = name.Find("-4CIF");  if(suffixPos != P_MAX_INDEX) break;
    suffixPos = name.Find("-480P");  if(suffixPos != P_MAX_INDEX) break;
   case 3: //VGA
   case 4: //4CIF
    suffixPos = name.Find("-SD");    if(suffixPos != P_MAX_INDEX) break;
   case 5: //SVGA
    suffixPos = name.Find("-720P");  if(suffixPos != P_MAX_INDEX) break;
   case 6: //XVGA
    suffixPos = name.Find("-768P");  if(suffixPos != P_MAX_INDEX) break;
    suffixPos = name.Find("-HD");    if(suffixPos != P_MAX_INDEX) break;
    suffixPos = name.Find("-16CIF"); if(suffixPos != P_MAX_INDEX) break;
   case 7: //SXGA
    suffixPos = name.Find("-FHD");   if(suffixPos != P_MAX_INDEX) break;
    suffixPos = name.Find("-16CIF"); if(suffixPos != P_MAX_INDEX) break;
    suffixPos = name.Find("-1080P"); if(suffixPos != P_MAX_INDEX) break;
   case 8: //16CIF
   default:
    break;
  }
  if(suffixPos == P_MAX_INDEX) enabledCodecs.AppendString(name);
  else capabilities.Remove(name);

// проверка кодека на соответствие размеру отправляемой картинки
// (меньше можно, больше нельзя) и создание списка допустимых кодеков
  suffixPos = P_MAX_INDEX;
  switch(videoSizeTx)
  {
   case 0: //QCIF
    suffixPos = name.Find("-CIF");   if(suffixPos != P_MAX_INDEX) break;
   case 1: //QVGA
    suffixPos = name.Find("-360P");  if(suffixPos != P_MAX_INDEX) break;
   case 2: //CIF
    suffixPos = name.Find("-4CIF");  if(suffixPos != P_MAX_INDEX) break;
    suffixPos = name.Find("-480P");  if(suffixPos != P_MAX_INDEX) break;
   case 3: //VGA
    suffixPos = name.Find("-SD");    if(suffixPos != P_MAX_INDEX) break;
   case 4: //4CIF
    suffixPos = name.Find("-720P");  if(suffixPos != P_MAX_INDEX) break;
   case 5: //SVGA
    suffixPos = name.Find("-768P");  if(suffixPos != P_MAX_INDEX) break;
   case 6: //XVGA
    suffixPos = name.Find("-HD");    if(suffixPos != P_MAX_INDEX) break;
   case 7: //HD 720
    suffixPos = name.Find("-FHD");   if(suffixPos != P_MAX_INDEX) break;
    suffixPos = name.Find("-1080P"); if(suffixPos != P_MAX_INDEX) break;
   case 8: //SXGA
    suffixPos = name.Find("-16CIF"); if(suffixPos != P_MAX_INDEX) break;
   case 9: //16CIF
   default:
    break;
  }
  if(suffixPos == P_MAX_INDEX) // добавляю в список допустимых
  {
   const char *p2pstr=name;
   tvCaps[tvNum]=strdup(p2pstr);
   tvNum++;
  }
 }

// Reorder the codecs we have
 capabilities.Reorder(enabledCodecs);

 for (PINDEX i = 0; i < capabilities.GetSize(); i++) 
 {
  if (capabilities[i].GetMainType() == H323Capability::e_Video)
  {
   capabilities[i].SetMediaFormatOptionInteger(OpalVideoFormat::MaxBitRateOption, videoInMaxBitRate);
   if((capabilities[i].GetFormatName().Find("H.264")!=P_MAX_INDEX) || (capabilities[i].GetFormatName().Find("VP8-")==0))
   {

    H323GenericVideoCapability *h264cap = (H323GenericVideoCapability *) &capabilities[i];
    h264cap->SetMaxBitRate(videoInMaxBitRate/100);
   }
  }
 }

/*	
	PINDEX audioNum = 1;
	PINDEX videoNum = 1;
	for (PINDEX i = 0; i < capabilities.GetSize(); i++) 
	{
		if (capabilities[i].GetMainType() == H323Capability::e_Audio)
		{
			config.SetString(CodecsConfigSection,
				psprintf("%u", audioNum++),
				capabilities[i].GetFormatName() + OnCodecSuffix);
		} 
		else if (capabilities[i].GetMainType() == H323Capability::e_Video)
		{
			config.SetString(VideoCodecsConfigSection,
				psprintf("%u", videoNum++),
				capabilities[i].GetFormatName() + OnCodecSuffix);
		}
	}
*/

	PTRACE(1, "MyPhone\tCapability Table:\n" << setprecision(4) << capabilities);
}

BOOL CMyPhoneEndPoint::OpenVideoChannel(H323Connection & connection,
					BOOL isEncoding,
					H323VideoCodec & codec)
{

 PVideoChannel   * channel = new PVideoChannel;
 PVideoDevice * displayDevice = NULL;
	
 if (isEncoding) 
 {
  // Transmitter part
  if(!autoStartTransmitVideo) return FALSE;
		
  codec.SetTxQualityLevel(config.GetInteger(VideoQualityConfigKey,15));
  codec.SetBackgroundFill(2);

  int videoOutMaxBitRate = config.GetInteger(VideoOutMaxbandWidthKey, 320);
  videoOutMaxBitRate = 1024 * PMAX(16, PMIN(10240, videoOutMaxBitRate));

  H323Channel * lchannel = codec.GetLogicalChannel();
  const H323Capability & capability = lchannel->GetCapability();
  PString cname = capability.GetFormatName();
  PINDEX suffixPos = cname.Find("H.263");
  if(suffixPos == P_MAX_INDEX) suffixPos = cname.Find("H.261");

  int videoSize = config.GetInteger(VideoOutSizeConfigKey, 2);
  int width=352, height=288;

  suffixPos = P_MAX_INDEX;
  switch(videoSize)
  {
   case 0: //QCIF
    width = 176; height = 144; break;
   case 1: //QVGA
    if(suffixPos == P_MAX_INDEX) { width = 320; height = 240; break; }
   case 2: //CIF
    width = 352; height = 288; break;
   case 3: //VGA
    if(suffixPos == P_MAX_INDEX) { width = 640; height = 480; break; }
   case 5: //SVGA
    if(suffixPos == P_MAX_INDEX) { width = 800; height = 600; break; }
   case 6: //XVGA
    if(suffixPos == P_MAX_INDEX) { width = 1024; height = 768; break; }
   case 4: //4CIF
    width = 704; height = 576; break;
   case 7: //HD 720
    if(suffixPos == P_MAX_INDEX) { width = 1280; height = 720; break; }
   case 8: //SXGA
    if(suffixPos == P_MAX_INDEX) { width = 1280; height = 1024; break; }
   case 9: //16CIF
    width = 1408; height = 1152; break;
   case 10: //UXGA
    if(suffixPos == P_MAX_INDEX) { width = 1600; height = 1200; break; }
   case 11: //HD 1080
    if(suffixPos == P_MAX_INDEX) { width = 1920; height = 1080; break; }
   default:
    break;
  }

  PTRACE(1, "Video device videoSize=" << videoSize << " width=" << width << " height=" << height);

  codec.SetVideoSize(width, height);
  width = codec.GetWidth(); 
  height = codec.GetHeight();

  
  int curMBR = codec.GetMaxBitRate();
  PTRACE(1,"codec.GetMaxBitRate()=" << curMBR << ", videoOutMaxBitRate=" << videoOutMaxBitRate << ", will set: " << (curMBR > videoOutMaxBitRate));
  if(curMBR > videoOutMaxBitRate) codec.SetMaxBitRate(videoOutMaxBitRate);
  PTRACE(1, "Accepted video device width=" << width << " height=" << height << " bitrate=" << PMIN(curMBR,videoOutMaxBitRate));

  int videoFramesPS = config.GetInteger(VideoFPSKey, 10);
  codec.SetGeneralCodecOption("Frame Rate",videoFramesPS);

  codec.SetTargetFrameTimeMs(1000/videoFramesPS);
  codec.SetGeneralCodecOption("Frame Time",90000/videoFramesPS);

//  OpalMediaFormat & mf = codec.GetWritableMediaFormat();
//  mf.SetOptionInteger("Frame Rate", videoFramesPS);
//  mf.SetOptionInteger("Frame Time", 90000/videoFramesPS);

  //Create grabber.
  bool NoDevice = false;
  PString deviceName = config.GetString(VideoDeviceConfigKey, deviceName);
  if (deviceName.IsEmpty()) 
  {
   PStringArray devices = PVideoInputDevice::GetDriversDeviceNames(VideoInputDriver);
   if (!devices.IsEmpty()) deviceName = devices[0];
   else NoDevice = true;
  }
		
  PVideoInputDevice * grabber = NULL;
  if (deviceName.Find("fake") == 0) NoDevice = true;
//  else if (deviceName.Find("screen") == 0) grabber = PVideoInputDevice::CreateDevice("ScreenVideo");
  else grabber = PVideoInputDevice::CreateDeviceByName(deviceName,VideoInputDriver);
		
  if (NoDevice || !grabber->Open(deviceName, FALSE) ||
      !grabber->SetFrameSize(width, height) || 
      !grabber->SetColourFormatConverter("YUV420P") || 
			!grabber->SetVFlipState(localFlip))
  {
   if(!NoDevice)
   {
    char sSrc[64];
    m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRVDEVSTR), S_SYSTEM, 
 			      (const char *) deviceName, 
				itoa(config.GetInteger(VideoSourceConfigKey,0), sSrc, 10));
    PTRACE(1, "Failed to open or configure the video device \"" << deviceName << '"');
   }

   if(grabber) delete grabber;
   grabber = PVideoInputDevice::CreateDevice("FakeVideo");
			grabber->SetColourFormat("YUV420P");
			grabber->SetVideoFormat(PVideoDevice::PAL);
			grabber->SetFrameSize(width, height);
			grabber->SetVFlipState(localFlip);
//			grabber->SetChannel(4);
			grabber->SetChannel((config.GetInteger(VideoSourceConfigKey, -1)+5)%7);
  }

  if(videoFramesPS >0 && videoFramesPS<30) 
   grabber->SetFrameRate(videoFramesPS);

  grabber->Start();

  channel->AttachVideoReader(grabber);
/*
  if (localVideo)
		{
			BOOL curVFlip = config.GetBoolean(VideoOutVFlipConfigKey, FALSE);
			BOOL curHFlip = config.GetBoolean(VideoOutHFlipConfigKey, FALSE);
			displayDevice = new CVideoOutputDevice(m_dialog, 
				connection.GetLocalPartyName(), curVFlip, curHFlip, TRUE, FALSE);
		}
  else 
*/
   displayDevice = PVideoOutputDevice::CreateDevice("NULLOutput");
 }
 else
 {
  // Receiver part
  if(!autoStartReceiveVideo) return FALSE;
  BOOL curVFlip = config.GetBoolean(VideoInVFlipConfigKey, FALSE);
  BOOL curHFlip = config.GetBoolean(VideoInHFlipConfigKey, FALSE);

  displayDevice = new CVideoOutputDevice( 
		m_vdlg, (LPCTSTR)m_dialog->FindContactName(connection), curVFlip,
		curHFlip, FALSE, localVideo);
 }
	
 if(displayDevice)
 {
  int width = codec.GetWidth();
  int height = codec.GetHeight();
//  if( m_dialog->autohideVideoPan && !m_dialog->showVideoPan)
//   m_dialog->ShowVideoPanels(TRUE);  // show Video panel if it's hiden
  displayDevice->SetColourFormat("RGB32");
  displayDevice->SetColourFormatConverter("YUV420P");
  displayDevice->SetFrameSize(width, height);
  //Give the video window refreshing class to the channel.
  channel->AttachVideoPlayer((PVideoOutputDevice *)displayDevice);
 }

 return codec.AttachChannel(channel,TRUE);
}

void CMyPhoneEndPoint::OnRTPStatistics(const H323Connection & connection,
									   const RTP_Session & session) const
{
	// Lock mutex
	if(!(((CMyPhoneEndPoint *)this)->m_StatLock.Lock(10))) return;
	// Reset statistic 
	m_stat.ibSent = m_stat.ibRcvd =0;
    // Getting Audio statistic part
	RTP_Session * asession = connection.GetSession(RTP_Session::DefaultAudioSessionID);
	if (asession != NULL)
	{
		int abSent=0, abRcvd=0;
		// If not sending or receiving Audio, memory read error happens.
		try{	abSent = asession->GetOctetsSent();	} catch(...) {abSent = 0;}
		try{	abRcvd = asession->GetOctetsReceived(); } catch(...) {abRcvd = 0;}
		m_stat.ibSent = abSent;
		m_stat.ibRcvd = abRcvd;
	}
	
	// Getting Video statistic part
	RTP_Session * vsession = connection.GetSession(RTP_Session::DefaultVideoSessionID);
	if (vsession != NULL)
	{
		int vbSent=0, vbRcvd=0;
		// If not sending or receiving Vidio 
		// mamory read error happends.
		try{	vbSent = vsession->GetOctetsSent();	} catch(...) {vbSent = 0;}
		try{	vbRcvd = vsession->GetOctetsReceived(); } catch(...) {vbRcvd = 0;}
		m_stat.ibSent += vbSent;
		m_stat.ibRcvd += vbRcvd;
	}
	
	m_stat.iDelay = connection.GetRoundTripDelay().GetInterval();
	
	m_stat.iSecs = (PTime() - connection.GetConnectionStartTime()).GetSeconds();
	//
	// Release mutex
	((CMyPhoneEndPoint *)this)->m_StatLock.Unlock();
}

BOOL CMyPhoneEndPoint::FindGatekeeper()
{
	if (GetGatekeeper() != NULL) {
		if (gatekeeper->IsRegistered()) // If registered, then unregister
			m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKUNREGSTR), S_SYSTEM, (const char *)gatekeeper->GetName());	
		RemoveGatekeeper();
	}
	
	if (!config.GetBoolean(UseGatekeeperConfigKey, FALSE))
		return TRUE;
	
	SetGatekeeperPassword(config.GetString(GatekeeperPassConfigKey));
	
	PString gkHost = config.GetString(GatekeeperHostConfigKey, "");
	PString gkid = config.GetString(GatekeeperIdConfigKey, "");
	PString iface = "";
	int iMode = config.GetInteger(DiscoverGatekeeperConfigKey, -1);
	switch(iMode)
	{
	case -1:
		return TRUE;
		break;
	case 0:
		gkHost = "";
		gkid = "";
		iface = "";
		break;
	case 1:
		gkid = "";
		iface = "";
		break;
	case 2:
		gkHost = "";
		iface = "";
		break;
	}

	m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKSRCHSTR), S_SYSTEM);	
	m_dialog->m_cstatus.UpdateWindow();
	AfxGetApp()->DoWaitCursor(1);
	if (UseGatekeeper(gkHost, gkid, iface)) {
		m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKREGSTR), S_SYSTEM, (const char *)gatekeeper->GetName());	
	}
	AfxGetApp()->DoWaitCursor(0);
	
	BOOL gkRequired = config.GetBoolean(RequireGatekeeperConfigKey, FALSE);
	
	if (GetGatekeeper() != NULL)
	{
		unsigned reason = gatekeeper->GetRegistrationFailReason();
		switch (reason) {
		case H323Gatekeeper::InvalidListener :
			m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRSTR),S_SYSTEM, (LPCTSTR)LoadStringLang(IDS_GKERRPORTSTR));
			break;
		case H323Gatekeeper::DuplicateAlias :
			m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRSTR),S_SYSTEM, (LPCTSTR)LoadStringLang(IDS_GKERRALIASSTR));
			break;
		case H323Gatekeeper::SecurityDenied :
			m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRSTR),S_SYSTEM, (LPCTSTR)LoadStringLang(IDS_GKERRSECSTR));
			break;
		case H323Gatekeeper::TransportError :
			m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRSTR),S_SYSTEM, (LPCTSTR)LoadStringLang(IDS_GKERRTRNSTR));
			break;
		default :
			if ((reason&H323Gatekeeper::RegistrationRejectReasonMask) != 0)
			{
				CString strRes;
				strRes.Format((LPCTSTR)LoadStringLang(IDS_GKERRUNSTR),(int) reason&(H323Gatekeeper::RegistrationRejectReasonMask-1));
				m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_ERRSTR), S_SYSTEM, (LPCTSTR) strRes);
			}
			break;
		}
	}
	else
	{
		if (!gkHost.IsEmpty())
		{
			if (!gkid.IsEmpty())
				m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKERRFINDSTR),
				S_SYSTEM, (const char *)gkid, (const char *)gkHost);
			else
				m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKERRFIND1STR),
				S_SYSTEM, (const char *)gkHost);
		}
		else {
			if (!gkid.IsEmpty())
				m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKERRFIND2STR),
				S_SYSTEM, (const char *)gkid);
			else
			{
				if (gkRequired)
					m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKERRFIND3STR),
					S_SYSTEM);
				else
					m_dialog->OutputStatusStr((LPCTSTR)LoadStringLang(IDS_GKERRFIND4STR),
					S_SYSTEM);
			}
		}
	}
	
	return !gkRequired; 
}

void CMyPhoneEndPoint::TranslateTCPAddress(PIPSocket::Address & localAddr,
										   const PIPSocket::Address & remoteAddr)
{
	if ((m_router!=NULL) && m_router.IsValid() && IsLocalAddress(localAddr) && !IsLocalAddress(remoteAddr))
		localAddr = m_router;
}
