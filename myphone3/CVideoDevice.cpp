/////////////////////////////////////////////////////////////////////////////
//
// CVideoDevice.cpp : Video Output Device 
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
#include <wingdi.h>
#include "resource.h"
#include "MyPhoneDlg.h"

#include "CVideoDevice.h"

#include "Multilang.h"		// Languages

CVideoOutputDevice::CVideoOutputDevice(
	CVideoDlg* wnd,
	const PString& remotePartyName,
		BOOL fFlipVertically, 
			BOOL fFlipHorizontally,
				BOOL isLocal,
					BOOL doLocalVideoPnP) :
   m_videoDlg(wnd),
   m_remotePartyName(remotePartyName),
   m_isLocal(isLocal),
   m_doLocalVideoPnP(doLocalVideoPnP),
   m_fVertFlip(fFlipVertically),
   m_fHorzFlip(fFlipHorizontally),
   
   m_bIsInitialized(false),
   m_pD3D(NULL),
   m_p3DDevice(NULL),
   m_p3DBackSurface(NULL),
   m_d3d_mode(2)

{
 m_x = m_y =0;  m_w = m_h = 200;
        m_videoDlg->ShowWindow(SW_SHOW);
        m_videoDlg->BringWindowToTop();
//	if(m_doLocalVideoPnP || !m_isLocal)
//		m_hWnd = m_wndDlg->GetDlgItem(IDC_VIDEOBITMAP);
                m_hWnd = m_videoDlg->GetDlgItem(IDC_STATIC);
//	else if (m_isLocal)
//		m_hWnd = m_wndDlg->GetDlgItem(IDC_VIDEOBITMAP1);

	m_cdc = m_videoDlg->GetDC();

// COLORREF col = RGB(100,100,100);
// FloodFill(m_cdc->GetSafeHdc(),0,0,col);

    m_hWnd->GetClientRect(&m_rect);
	// Set for PIP
	m_rect.right = m_doLocalVideoPnP&&m_isLocal ? m_rect.left/2 : m_rect.left;
	m_rect.bottom = m_doLocalVideoPnP&&m_isLocal ? m_rect.bottom/2 : m_rect.bottom;


	m_bmi.bmiHeader.biSize = sizeof(m_bmi.bmiHeader);
	m_bmi.bmiHeader.biWidth = frameWidth;
	m_bmi.bmiHeader.biHeight = -(int)frameHeight;
	m_bmi.bmiHeader.biPlanes = 1;
	m_bmi.bmiHeader.biBitCount = 32;
	m_bmi.bmiHeader.biCompression = BI_RGB;
	m_bmi.bmiHeader.biXPelsPerMeter = 0;
	m_bmi.bmiHeader.biYPelsPerMeter = 0;
	m_bmi.bmiHeader.biClrImportant = 0;
	m_bmi.bmiHeader.biClrUsed = 0;
	m_bmi.bmiHeader.biSizeImage = frameStore.GetSize();

/*
	if(m_wndDlg!=NULL)
	{
		CString vStr;
		if(m_isLocal)
		{
			vStr.Format((LPCTSTR)LoadStringLang(IDS_LOCALSTR), (LPCTSTR) m_remotePartyName);
			m_wndDlg->GetDlgItem(IDC_SLVIDEO)->SetWindowText(vStr.GetBuffer(0));
		}
		else
		{
			vStr.Format((LPCTSTR)LoadStringLang(IDS_REMOTESTR), (LPCTSTR) m_remotePartyName);
			m_wndDlg->GetDlgItem(IDC_SRVIDEO)->SetWindowText(vStr.GetBuffer(0));
		}
	}
*/
}

CVideoOutputDevice::~CVideoOutputDevice()
{
  m_videoDlg->ShowWindow(SW_HIDE);
  PWaitAndSignal m(mutex);
  Close();
}

BOOL CVideoOutputDevice::Open(const PString & /*deviceName*/,
                              BOOL /*startImmediate*/)
{
  return TRUE;
}

BOOL CVideoOutputDevice::IsOpen()
{
  return m_hWnd != NULL;
}

BOOL CVideoOutputDevice::Close()
{
  return TRUE;
}

PStringList CVideoOutputDevice::GetDeviceNames() const
{
  PStringList  devlist;
  devlist.AppendString(GetDeviceName());
  return devlist;
}

BOOL CVideoOutputDevice::FrameComplete()
{

  PWaitAndSignal m(mutex);

  if (m_hWnd == NULL)
    return FALSE;

  if (m_videoDlg->d3d_mode!=0 && m_d3d_mode!=1)
  { 
   m_d3d_mode=1; 
   Release_d3d();
   if(Init_d3d(false)==false)
   { m_d3d_mode=1; m_videoDlg->d3d_mode=0; } 
  }

  if (m_videoDlg->d3d_mode!=1 && m_d3d_mode!=0)
   { 
    m_d3d_mode=0; 
    Release_d3d(); 
    RECT vRect;
    vRect.left = m_x; vRect.top = m_y; vRect.right = m_w; vRect.bottom = m_h;
    m_videoDlg->MoveWindow(&vRect);
   }

  if(m_bIsInitialized == TRUE && m_d3d_mode==1)
  {
   D3DLOCKED_RECT lr;

/*
                  float scale=1.0*GetSystemMetrics(SM_CXSCREEN)/frameWidth;
                  if(scale>1.0*GetSystemMetrics(SM_CYSCREEN)/frameHeight) 
                    scale=1.0*GetSystemMetrics(SM_CYSCREEN)/frameHeight;
                  int width = scale*frameWidth;
                  int height = scale*frameHeight;
		  int x = (GetSystemMetrics(SM_CXSCREEN)-width)>>1;
                  int y = (GetSystemMetrics(SM_CYSCREEN)-height)>>1;
*/
   int x, y, width  = GetSystemMetrics(SM_CXSCREEN), // расчёт чуть "легче", хотя особой разницы нет
             height = GetSystemMetrics(SM_CYSCREEN);
   if(frameWidth*height > width*frameHeight){
     int height0=height; height = width*frameHeight/frameWidth;
     x = 0; y = (height0-height)>>1;
   }
   else if(frameWidth*height<width*frameHeight){
     int width0=width; width=height*frameWidth/frameHeight;
     x=(width0-width)>>1; y=0;
   }
   else { x=0; y=0; }

   m_p3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
  
   m_p3DBackSurface->LockRect(&lr,NULL,0);
   if((frameWidth<<2) != lr.Pitch) {
     BYTE * dst=static_cast<BYTE *>(lr.pBits);
     BYTE * src=frameStore.GetPointer();
     unsigned amount=frameWidth<<2;
     for(unsigned i=0;i<frameHeight;i++) { memcpy(dst,src,amount); src+=amount; dst+=lr.Pitch; }
   } else
   memcpy(static_cast<BYTE *>(lr.pBits),frameStore.GetPointer(),frameStore.GetSize());
   m_p3DBackSurface->UnlockRect();

   HRESULT Hr;
   LPDIRECT3DSURFACE9 p3DTargetSurface;
   Hr = m_p3DDevice->GetRenderTarget(0, &p3DTargetSurface);
   if( FAILED(Hr) ) return false;
   RECT r;
   r.left=x; r.top=y; r.right=x+width-1; r.bottom=y+height-1;
   m_p3DDevice->StretchRect(m_p3DBackSurface,NULL,p3DTargetSurface,&r,D3DTEXF_LINEAR);
   p3DTargetSurface->Release();

   Hr=m_p3DDevice->Present( NULL, NULL, NULL, NULL );
   if(FAILED(Hr)){
      Release_d3d(); m_videoDlg->d3d_mode=0;
   }
   return TRUE;
  }


  if((frameHeight << 16) + frameWidth != windowAlreadyMoved) // just opened or frame size changed
  {
    RECT vRect, wRect, dRect; // video, window, desktop work area
    unsigned newW, newH, maxAvailW, maxAvailH;

    if(SystemParametersInfo(SPI_GETWORKAREA, NULL, &dRect, 0))
    { maxAvailW = dRect.right-dRect.left+1;
      maxAvailH = dRect.bottom-dRect.top+1;
    } else
    { maxAvailW = GetSystemMetrics(SM_CXSCREEN);
      maxAvailH = GetSystemMetrics(SM_CYSCREEN) - 48;
      dRect.top=0; dRect.left=0;
    }

    m_videoDlg->GetClientRect(&vRect); m_videoDlg->GetWindowRect(&wRect);

    int willLostX = wRect.right-wRect.left-vRect.right;
    int willLostY = wRect.bottom-wRect.top-vRect.bottom;

    maxAvailW -= willLostX; maxAvailH -= willLostY;


    if(frameWidth*3 <= maxAvailW && (frameHeight<<1) <= maxAvailH) // too small: 2x zoom
    {
      newW = (frameWidth<<1); newH = (frameHeight<<1);
    }
    else if(frameWidth > maxAvailW+3 || frameHeight > maxAvailH+3) // too big: zoom out
    {
      if(frameWidth*maxAvailH > frameHeight*maxAvailW) // frame more wide than device: fit width first
      {
        newW = maxAvailW; newH = newW*frameHeight/frameWidth;
      }
      else // frame more high than device (or equal): fit height first
      {
        newH = maxAvailH; newW = newH*frameWidth/frameHeight;
      }
    }
    else // keep size unchanged
    {
      newW = frameWidth; newH = frameHeight;
    }

    newW+=willLostX; newH+=willLostY;

    wRect.left = dRect.left+((maxAvailW-newW)>>1);
    wRect.top = dRect.top+((maxAvailH-newH)>>1);
    wRect.right = wRect.left + newW - 1;
    wRect.bottom = wRect.top + newH - 1;
    m_videoDlg->MoveWindow(&wRect);

    windowAlreadyMoved = (frameHeight << 16) + frameWidth;
  }

/*  RECT vRect;
  m_videoDlg->GetClientRect(&vRect);
  if(frameWidth > vRect.right || frameHeight > vRect.bottom)
  {
   RECT wRect;
   m_videoDlg->GetWindowRect(&wRect);
   int dx = frameWidth-vRect.right;
   int dy = frameHeight-vRect.bottom;
//   vRect.left = wRect.left; vRect.top = wRect.top;
//   vRect.right = wRect.right+dx; vRect.bottom = wRect.bottom+dy;
   wRect.right+=dx; wRect.bottom+=dy;
   
   if(frameWidth>=GetSystemMetrics(SM_CXSCREEN) || frameHeight>=GetSystemMetrics(SM_CYSCREEN)) {}
//   else m_videoDlg->MoveWindow(&vRect);
   else m_videoDlg->MoveWindow(&wRect);
  }
*/
//        m_videoDlg->ShowWindow(SW_SHOW);
  Redraw(m_cdc->GetSafeHdc());

  return TRUE;

}


void CVideoOutputDevice::Redraw(HDC curDC)
{
	if(curDC)
	{
		::SetStretchBltMode(curDC, HALFTONE);

		// Important note: 
		// Images height is measured from the bottom in the framestore

		int result=0;
/*
		if (m_doLocalVideoPnP && !m_isLocal && m_wndDlg!=NULL && !m_wndDlg->hidePnP)
		{
			// Upper right part of image
			result = StretchDIBits(curDC, 
				frameWidth/2, 0, frameWidth/2, frameHeight/2 , 
				frameWidth/2, frameHeight/2, frameWidth/2, frameHeight/2, 
				frameStore.GetPointer(), &m_bmi, DIB_RGB_COLORS, SRCCOPY);
			// bottom full width part of image
			result = StretchDIBits(curDC, 
				0, frameHeight/2, frameWidth, frameHeight/2 , 
				0, 0, frameWidth, frameHeight/2, 
				frameStore.GetPointer(), &m_bmi, DIB_RGB_COLORS, SRCCOPY);
		}
		else if (m_doLocalVideoPnP && m_isLocal && m_wndDlg!=NULL && !m_wndDlg->hidePnP) 
		{
			result = StretchDIBits(curDC, 
				0, 0, frameWidth/2, frameHeight/2, 
				0, 0, frameWidth, frameHeight, 
				frameStore.GetPointer(), &m_bmi, DIB_RGB_COLORS, SRCCOPY);

		} else if (!m_isLocal || (m_isLocal && !m_doLocalVideoPnP)) 
*/
		{
                 RECT vRect;
                 m_videoDlg->GetClientRect(&vRect);
                 if(frameWidth == vRect.right || frameHeight == vRect.bottom)
                 {
                  result = SetDIBitsToDevice(curDC,
                  0, 0, frameWidth, frameHeight,
                               0, 0, 0, frameHeight,
                               frameStore.GetPointer(),  &m_bmi, DIB_RGB_COLORS);
                  RECT wRect;
                  m_videoDlg->GetWindowRect(&wRect);
                  m_x=wRect.left; m_y=wRect.top; m_w=wRect.right; m_h=wRect.bottom;
                 }

                 else
                 {
                  float scale=(vRect.right+1.0)/frameWidth;
                  if(scale>(vRect.bottom+1.0)/frameHeight) scale=(vRect.bottom+1.0)/frameHeight;
                  int width = scale*frameWidth;
                  int height = scale*frameHeight;
                  int x = ((vRect.right+1)-width)>>1;
                  int y = ((vRect.bottom+1)-height)>>1;
                  RECT wRect;
                  m_videoDlg->GetWindowRect(&wRect);
                  if(m_x!=wRect.left || m_y!=wRect.top || 
                     m_w!=wRect.right || m_h!=wRect.bottom)
                  {
                   m_x=wRect.left; m_y=wRect.top; m_w=wRect.right; m_h=wRect.bottom;
                   PatBlt(curDC,0,0,vRect.right,vRect.bottom,BLACKNESS);
                  }
                  result = StretchDIBits(curDC,
                            x, y, width, height,
                            0, 0, frameWidth, frameHeight,
                            frameStore.GetPointer(),  &m_bmi, DIB_RGB_COLORS,SRCCOPY);
                 }

		}

        if (result == GDI_ERROR) {
             lastError = ::GetLastError();
             PTRACE(2, "VidOut\tDrawing image failed, error=" << lastError);
		} 
	}

}

BOOL CVideoOutputDevice::SetFrameSize( unsigned width, unsigned height )
{
	if (width == 0 || height == 0)
		return TRUE;

    if (m_hWnd != NULL && width == frameWidth && height == frameHeight)
      return TRUE;

	PTRACE(4,"VID\tSetting Frame Size w " << width << " h " << height);

    if (!PVideoOutputDeviceRGB::SetFrameSize(width, height))
      return FALSE;

    m_bmi.bmiHeader.biWidth = frameWidth;
    m_bmi.bmiHeader.biHeight = m_fVertFlip ? frameHeight : -(int)frameHeight;
    m_bmi.bmiHeader.biSizeImage = frameStore.GetSize();

	return TRUE;
}

BOOL CVideoOutputDevice::SetColourFormat(const PString & colourFormat)
{
  PWaitAndSignal m(mutex);

  if (((colourFormat *= "BGR24") || (colourFormat *= "BGR32")) &&
                PVideoOutputDeviceRGB::SetColourFormat(colourFormat)) {
    m_bmi.bmiHeader.biBitCount = (WORD)(bytesPerPixel*8);
    m_bmi.bmiHeader.biSizeImage = frameStore.GetSize();
    return TRUE;
  }

  return FALSE;
}

BOOL CVideoOutputDevice::Init_d3d(bool windowed)
{
   m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
   if( m_pD3D == NULL ) return false;
   HRESULT Hr;
   D3DDISPLAYMODE d3ddm = { 0 };
   Hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
   if( FAILED(Hr) ) return false;
   m_ColorFormat = d3ddm.Format;
   Hr = m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,
                                D3DDEVTYPE_HAL,
                                m_ColorFormat,
                                m_ColorFormat,
                                FALSE);
   if( FAILED(Hr) ) return false;
   D3DPRESENT_PARAMETERS d3dpp = { 0 };
   d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;; //D3DSWAPEFFECT_FLIP
   d3dpp.Windowed = windowed;
   if(windowed)
   {
	d3dpp.hDeviceWindow = HWND(*m_hWnd);
    d3dpp.BackBufferWidth = frameWidth;
    d3dpp.BackBufferHeight = frameHeight;
   }
   else 
   {
    d3dpp.BackBufferWidth        = GetSystemMetrics(SM_CXSCREEN);
    d3dpp.BackBufferHeight       = GetSystemMetrics(SM_CYSCREEN);
   }
   d3dpp.BackBufferCount = 1;
   d3dpp.BackBufferFormat = m_ColorFormat;

   d3dpp.EnableAutoDepthStencil = FALSE;
   d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
   d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
   Hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                             D3DDEVTYPE_HAL,
                             HWND(*m_videoDlg),
                             D3DCREATE_HARDWARE_VERTEXPROCESSING,
                             &d3dpp,
                             &m_p3DDevice);
   if( FAILED(Hr) ) Hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                             D3DDEVTYPE_HAL,
                             HWND(*m_videoDlg),
                             D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                             &d3dpp,
                             &m_p3DDevice);
   if( FAILED(Hr) ) return false;       

   Hr = m_p3DDevice->CreateOffscreenPlainSurface(frameWidth, frameHeight, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_p3DBackSurface, NULL);
   if( FAILED(Hr) ) return false;

   m_bIsInitialized = true;
   return true;
}

void CVideoOutputDevice::Release_d3d()
{
   m_bIsInitialized = false;
   Sleep(1000);
   if( m_p3DBackSurface != NULL ) m_p3DBackSurface->Release();
   m_p3DBackSurface = NULL;
   if( m_p3DDevice != NULL ) m_p3DDevice->Release();
   m_p3DDevice = NULL;
   if( m_pD3D != NULL ) m_pD3D->Release();
   m_pD3D = NULL;
}

