// MyPhoneEndPoint.h: interface for the CMyPhoneEndPoint class.
//						H323's End Point and Connection
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


#if !defined(AFX_MYPHONEENDPOINT_H__B1AF53B8_E3C6_4590_BBC8_6614922EC493__INCLUDED_)
#define AFX_MYPHONEENDPOINT_H__B1AF53B8_E3C6_4590_BBC8_6614922EC493__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ptlib.h>
#include <ptlib/sound.h>
#include <ptlib/video.h>

#include <h323.h>

class CMyPhoneDlg;
class CVideoDlg;

// Structure for stored connection statistics
typedef struct
{
	UINT iSecs;		// talk duration
	UINT ibSent;	// bytes sent
	UINT ibRcvd;	// bytes received
	UINT iDelay;	// rount trip delay
} MyStat;

class CMyPhoneEndPoint : public H323EndPoint  
{
public:
	CMyPhoneEndPoint();
	virtual ~CMyPhoneEndPoint();
    BOOL OnIncomingCall(H323Connection &, const H323SignalPDU &, H323SignalPDU &);
	virtual H323Connection::AnswerCallResponse OnAnswerCall(H323Connection &, const PString & caller,const H323SignalPDU &,H323SignalPDU &);
	virtual void OnConnectionEstablished(H323Connection & connection, const PString & token);
	virtual void OnConnectionCleared(H323Connection & connection, const PString & clearedCallToken);
    BOOL OpenAudioChannel(H323Connection &, BOOL, unsigned, H323AudioCodec &);
    BOOL OpenVideoChannel(H323Connection &, BOOL, H323VideoCodec &);
    BOOL OnStartLogicalChannel(H323Connection &, H323Channel &);
    void OnClosedLogicalChannel(H323Connection &, const H323Channel &);
	void OnRTPStatistics(const H323Connection &, const RTP_Session &) const;
    void TranslateTCPAddress(PIPSocket::Address &localAddr, const PIPSocket::Address &remoteAddr);
	BOOL CMyPhoneEndPoint::FindGatekeeper();
	//
	BOOL Initialise(CMyPhoneDlg * dlg, CVideoDlg * vdlg);
	void LoadCapabilities(); 
	virtual H323Connection * CreateConnection(unsigned refID);
	void OnLogicalChannel(const H323Channel & channel, unsigned txStrID, unsigned rxStrID);

public:
	CString goodName;
	mutable MyStat	m_stat;

	CMyPhoneDlg * m_dialog;
        CVideoDlg * m_vdlg;
	PConfig config;

	unsigned recBufSize;
	unsigned playBufSize;
//	unsigned videoSize;
//	unsigned videoWidth;
//	unsigned videoHeight;

	BOOL autoStartTransmitVideo;
	BOOL autoStartReceiveVideo;
	BOOL localVideo;
	BOOL localFlip;
    BOOL h261Enable;
    BOOL h263Enable;  // for future use
	PIPSocket::Address m_router;
	static PString VideoInputDriver;

	BOOL m_fSilenceOn;
	BOOL m_fAECOn;
	int  m_fAGCOn;
	BOOL m_fAutoAnswer;
	BOOL m_fNoFastStart;
	BOOL m_fDoH245Tunnelling;
	BOOL m_fDtmfAsString;

	CMutex m_StatLock;

	inline void DisableH245Tunnelling(BOOL bDisable) { m_fDoH245Tunnelling = !bDisable; }
	inline BOOL DisableH245Tunnelling() { return !m_fDoH245Tunnelling; }

	friend class CMyPhoneDlg;
	friend class CMyPhoneConnection;

};

class CMyPhoneConnection : public H323Connection
{
  PCLASSINFO(CMyPhoneConnection, H323Connection);

  public:
    CMyPhoneConnection(
		CMyPhoneDlg* wnd,
		CMyPhoneEndPoint & ep,
		unsigned callReference,
		unsigned options);

    // overrides from H323Connection
    BOOL OnAlerting(const H323SignalPDU &, const PString &);
    void OnUserInputString(const PString &);

  protected:
    CMyPhoneEndPoint & endpoint;
    CMyPhoneDlg * m_dialog;
};

#endif // !defined(AFX_MYPHONEENDPOINT_H__B1AF53B8_E3C6_4590_BBC8_6614922EC493__INCLUDED_)
