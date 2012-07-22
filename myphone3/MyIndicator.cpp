///////////////////////////////////////////////////////////////////////////////
// MyIndicator.cpp : implementation file for my Level Indicator class
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
#include "myphone.h"
#include "MyIndicator.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
COLORREF	CMyIndicator::clrBg;
COLORREF	CMyIndicator::clrFg;
COLORREF	CMyIndicator::clrSFg;
//
const UINT MaxScaleFactor = 10000;
/////////////////////////////////////////////////////////////////////////////
// CMyIndicator

CMyIndicator::CMyIndicator()
{
	logarithmic = FALSE;
	maximumLevel = 32767;
	markerPosition = 0;
	currentPosition = 0;

	// some default colors
	CMyIndicator::clrBg = RGB(0x00, 0x00, 0x00); 
	CMyIndicator::clrFg = RGB(0x19, 0x7B, 0x30); 
	CMyIndicator::clrSFg = RGB(0xC0, 0xC0, 0xC0); 
}

CMyIndicator::~CMyIndicator()
{
}

void CMyIndicator::Reset()
{
	logarithmic = FALSE;
	maximumLevel = 32767;
	markerPosition = 0;
	currentPosition = 0;
	Invalidate();
}

void CMyIndicator::SetActive(BOOL isActive)
{
	active=isActive;
}

void CMyIndicator::SetMaximumLevel(UINT level)
{
  if (maximumLevel == level)
    return;

  maximumLevel = level;
  markerPosition = ScaleLevel(markerPosition);
  currentPosition = ScaleLevel(currentPosition);
  Invalidate();
}

void CMyIndicator::SetMarkerLevel(UINT level)
{
  unsigned position = ScaleLevel(level);

  if (markerPosition == position)
    return;

  markerPosition = position;
  Invalidate();
}


void CMyIndicator::SetCurrentLevel(UINT level)
{
  unsigned position = ScaleLevel(level);

  if (currentPosition == position)
    return;

  currentPosition = position;
  Invalidate();
}


unsigned CMyIndicator::ScaleLevel(UINT level)
{
  if (level > maximumLevel)
    level = maximumLevel;

  if (logarithmic)
    return level*MaxScaleFactor/maximumLevel;

  // If not logirathmic, then we have to make it so!
  return (unsigned)(log10(9.0*level/maximumLevel+1)*MaxScaleFactor);
}

/*
void CMyIndicator::OnRedraw(PCanvas & canvas)
{
  PRect bounds = canvas.GetDrawingBounds();

  PDIMENSION level = currentPosition*bounds.Width()/MaximumVuScaleFactor;

  PRect leftSide = bounds;
  leftSide.SetWidth(level);

  PRect rightSide = bounds;
  rightSide.SetLeft(level);

  canvas.DrawRect(rightSide);

  canvas.SetFillFgColour(GetForegroundColour());
  canvas.DrawRect(leftSide);

  if (markerPosition > 0) {
    PDIMENSION mark = markerPosition*bounds.Width()/MaximumVuScaleFactor;
    if (mark < level)
      canvas.SetPenFgColour(GetBackgroundColour());
    else
      canvas.SetPenFgColour(GetForegroundColour());

    canvas.SetPenWidth(2);
    canvas.DrawLine(mark, bounds.Top(), mark, bounds.Bottom());
  }
}


void MainWindow::OnUpdateVU(PTimer &, INT)
{
  H323Connection * connection = endpoint.FindConnectionWithLock(currentCallToken);
  if (connection == NULL)
    return;

  H323Channel * channel = connection->FindChannel(RTP_Session::DefaultAudioSessionID, TRUE);
  if (channel != NULL) {
    H323AudioCodec * codec = (H323AudioCodec *)channel->GetCodec();
    speakerVU->SetCurrentLevel(codec->GetAverageSignalLevel());
  }

  channel = connection->FindChannel(RTP_Session::DefaultAudioSessionID, FALSE);
  if (channel != NULL) {
    H323AudioCodec * codec = (H323AudioCodec *)channel->GetCodec();
    BOOL inTalkBurst;
    unsigned threshold;
    if (codec->GetSilenceDetectionMode(&inTalkBurst, &threshold) != H323AudioCodec::NoSilenceDetection) {
      microphoneVU->SetMarkerLevel(threshold);
      microphoneVU->SetForegroundColour(inTalkBurst ? PColour::Red : PColour::Magenta);
    }
    microphoneVU->SetCurrentLevel(codec->GetAverageSignalLevel());
  }

  connection->Unlock();
}
*/

BEGIN_MESSAGE_MAP(CMyIndicator, CStatic)
	//{{AFX_MSG_MAP(CMyIndicator)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyIndicator message handlers

void CMyIndicator::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	COLORREF	clrCur = (active?clrFg:clrSFg);
	
	RECT bounds;
	GetClientRect(&bounds);
	dc.Rectangle(&bounds);

	UINT level = currentPosition*bounds.right/MaxScaleFactor;
	
	RECT leftSide = bounds;
	leftSide.left +=1;
	leftSide.right = level+1;
	leftSide.top += 1;
	leftSide.bottom -=1;
	
//	RECT rightSide = bounds;
//	rightSide.left = level;
	
//	dc.Rectangle(&rightSide);
	if(level>0)
		dc.FillSolidRect(&leftSide, clrCur);
	
	if (markerPosition > 0) {
		UINT mark = markerPosition*bounds.right/MaxScaleFactor;
		if (mark < level)
			dc.FillSolidRect(mark, leftSide.top, 2, leftSide.bottom-1, clrBg);
		else
			dc.FillSolidRect(mark, leftSide.top, 2, leftSide.bottom-1, clrCur);
	}

}
