/////////////////////////////////////////////////////////////////////////////
//
// CVideoDevice.h : interface for Video Output Device
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

#ifndef CVIDEODEVICE_H
#define CVIDEODEVICE_H

#include <ptlib.h>
#include <ptlib/video.h>

//#include "MyPhoneDlg.h"
//#include <codecs.h>

#include <d3d9.h>
#include <d3dx9.h>

class CVideoOutputDevice : public PVideoOutputDeviceRGB
{
public:

	CVideoOutputDevice(CMyPhoneDlg* wnd,
		const PString& remotePartyName, 
		BOOL fFlipVertically = FALSE, 
		BOOL fFlipHorizontally = FALSE,
		BOOL isLocal = FALSE,
		BOOL doLocalVideoPnP = FALSE);

        CVideoOutputDevice(CVideoDlg* wnd,
                const PString& remotePartyName,
                BOOL fFlipVertically = FALSE,
                BOOL fFlipHorizontally = FALSE,
                BOOL isLocal = FALSE,
                BOOL doLocalVideoPnP = FALSE);

	~CVideoOutputDevice();

    virtual BOOL Open(
      const PString & deviceName,   /// Device name to open
      BOOL startImmediate = TRUE    /// Immediately start device
	  );

    virtual BOOL IsOpen();

	virtual BOOL Close();

    virtual PStringList GetDeviceNames() const;

	// Resizing frames
	BOOL SetFrameSize(
      unsigned width,   /// New width of frame
      unsigned height   /// New height of frame
    );

    BOOL SetColourFormat(
      const PString & colourFormat // New colour format for device.
    );
/*
BOOL SetFrameData(unsigned x, unsigned y,
                                         unsigned width, unsigned height,
                                         const BYTE * data,
                                         BOOL endFrame);
*/
    BOOL FrameComplete();

    void Redraw(HDC curDC);

    BOOL Init_d3d(bool windowed);
    void Release_d3d();

protected:
	CMyPhoneDlg*	m_wndDlg;
        CVideoDlg*	m_videoDlg;
	CWnd*			m_hWnd;
    CDC*            m_cdc;
	PString			m_remotePartyName;
	RECT            m_rect;
	BITMAPINFO		m_bmi;
    PMutex			mutex;

	BOOL			m_fVertFlip;
	BOOL			m_fHorzFlip;
	BOOL			m_isLocal;
	BOOL			m_doLocalVideoPnP;
	int	m_x;
	int	m_y;
	int	m_w;
	int	m_h;

   BOOL m_bIsInitialized;
   D3DFORMAT m_ColorFormat;
   LPDIRECT3D9 m_pD3D;
   LPDIRECT3DDEVICE9 m_p3DDevice;
   LPDIRECT3DSURFACE9 m_p3DBackSurface;
   int m_d3d_mode;
   int windowAlreadyMoved;
   //
//   LPDIRECT3DVERTEXBUFFER9 m_p3DVertices[MAX_BUFFERS];
//   LPDIRECT3DTEXTURE9 m_p3DTextures[MAX_BUFFERS];

};

#endif
