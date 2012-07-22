#define P_FORCE_STATIC_PLUGIN

#include <ptlib.h>

#if P_VIDEO

#include <ptlib/vconvert.h>

#if defined(_WIN32) && !defined(P_FORCE_STATIC_PLUGIN)
#error "vscreen.cxx must be compiled without precompiled headers"
#endif
#include <ptlib/pluginmgr.h>
#include <ptlib/pprocess.h>
#include <ptlib/videoio.h>
#include <ptclib/delaychan.h>

#ifdef __MACOSX__
namespace PWLibStupidOSXHacks {
  int loadScreenVideoStuff;
};
#endif

/** This class defines a video input device that
    generate screen capture video. No resizing, clipping only.
*/
class PVideoInputDevice_ScreenVideo : public PVideoInputDevice
{
 PCLASSINFO(PVideoInputDevice_ScreenVideo, PVideoInputDevice);
 public:
  /** Create a new video input device.
   */
    PVideoInputDevice_ScreenVideo();


    /**Open the device given the device name.
      */
    BOOL Open(
      const PString & deviceName,   /// Device name to open
      BOOL startImmediate = TRUE    /// Immediately start device
    );

    /**Determine of the device is currently open.
      */
    BOOL IsOpen() ;

    /**Close the device.
      */
    BOOL Close();

    /**Start the video device I/O.
      */
    BOOL Start();

    /**Stop the video device I/O capture.
      */
    BOOL Stop();

    /**Determine if the video device I/O capture is in progress.
      */
    BOOL IsCapturing();

    /**Get a list of all of the drivers available.
      */
    static PStringList GetInputDeviceNames();

    virtual PStringList GetDeviceNames() const
      { return GetInputDeviceNames(); }

    /**Get the maximum frame size in bytes.

       Note a particular device may be able to provide variable length
       frames (eg motion JPEG) so will be the maximum size of all frames.
      */
    virtual PINDEX GetMaxFrameBytes();

    /**Grab a frame. 

       There will be a delay in returning, as specified by frame rate.
      */
    virtual BOOL GetFrameData(
      BYTE * buffer,                 /// Buffer to receive frame
      PINDEX * bytesReturned = NULL  /// Optional bytes returned.
    );

    /**Grab a frame.

       Do not delay according to the current frame rate.
      */
    virtual BOOL GetFrameDataNoDelay(
      BYTE * buffer,                 /// Buffer to receive frame
      PINDEX * bytesReturned = NULL  /// OPtional bytes returned.
    );

    /**Generate a static image, containing a constant field of grey.
     */
    void GrabScreenDCImage(BYTE *resFrame);

    /**Set the video format to be used.

       Default behaviour sets the value of the videoFormat variable and then
       returns the IsOpen() status.
    */
    virtual BOOL SetVideoFormat(
      VideoFormat videoFormat   /// New video format
    );

    /**Set the colour format to be used.

       Default behaviour sets the value of the colourFormat variable and then
       returns the IsOpen() status.
    */
    virtual BOOL SetColourFormat(
      const PString & colourFormat   // New colour format for device.
    );
    
    /**Set the video frame rate to be used on the device.

       Default behaviour sets the value of the frameRate variable and then
       return the IsOpen() status.
    */
    virtual BOOL SetFrameRate(
      unsigned rate  /// Frames per second
    );
         
    /**Get the minimum & maximum size of a frame on the device.

       Default behaviour returns the value 1 to UINT_MAX for both and returns
       FALSE.
    */
    virtual BOOL GetFrameSizeLimits(
      unsigned & minWidth,   /// Variable to receive minimum width
      unsigned & minHeight,  /// Variable to receive minimum height
      unsigned & maxWidth,   /// Variable to receive maximum width
      unsigned & maxHeight   /// Variable to receive maximum height
    ) ;

    /**Set the frame size to be used.

       Default behaviour sets the frameWidth and frameHeight variables and
       returns the IsOpen() status.
    */
    virtual BOOL SetFrameSize(
      unsigned width,   /// New width of frame
      unsigned height   /// New height of frame
    );
         
    void ClearMapping() { return ; }

    /**Try all known video formats & see which ones are accepted by the video driver
     */
    virtual BOOL TestAllFormats()
      { return TRUE; }

 protected:
   unsigned grabCount;
   PINDEX   videoFrameSize;
   PINDEX   scanLineWidth;
   PINDEX   bytesPerPixel; // 2==YUV420P, 3=RGB24, 4=RGB32
   PAdaptiveDelay m_Pacing;

   HDC h_screendc;
   HDC h_compatdc;
   HDC h_zoomdc;
   HBITMAP h_bmp;
   HBITMAP h_bmpOld;
   HBITMAP h_zbmp;
   HBITMAP h_zbmpOld;
   BITMAPINFO bmi;
//   BITMAP bmp;
   char *tempFrame;
};

PCREATE_VIDINPUT_PLUGIN(ScreenVideo);

   PString dName;
   long screenLeft;
   long screenTop;
   long screenWidth;
   long screenHeight;
   int useZoom;
   DWORD mNum;

///////////////////////////////////////////////////////////////////////////////
// PVideoInputDevice_ScreenVideo


PVideoInputDevice_ScreenVideo::PVideoInputDevice_ScreenVideo()
{
  tempFrame=NULL;
  mNum=0;
  SetColourFormat("BGR32");

  grabCount = 0;
  SetFrameRate(10);

  h_screendc = NULL;
  h_compatdc = NULL;
  h_zoomdc = NULL;
  screenWidth = 0;
  screenHeight = 0;
  screenLeft = 0;
  screenTop = 0;
  h_bmpOld=NULL; h_bmp=NULL;
  h_zbmpOld=NULL; h_zbmp=NULL;
  useZoom=0;
}



BOOL CALLBACK MyMonitorEnumProc(
  HMONITOR hMonitor,    // handle to display monitor
  HDC hdcMonitor,       // handle to monitor DC
  LPRECT lprcMonitor,   // monitor intersection rectangle
  LPARAM dwData         // data
)
{
 PString mName=PString("monitor")+PString(mNum);
 if(mName==dName)
 { 
  screenLeft=lprcMonitor->left; screenTop=lprcMonitor->top;
  screenWidth=GetDeviceCaps(hdcMonitor, HORZRES);
  screenHeight=GetDeviceCaps(hdcMonitor, VERTRES);
 }
 mName=PString("zmonitor")+PString(mNum);
 if(mName==dName)
 { 
  screenLeft=lprcMonitor->left; screenTop=lprcMonitor->top;
  screenWidth=GetDeviceCaps(hdcMonitor, HORZRES);
  screenHeight=GetDeviceCaps(hdcMonitor, VERTRES);
  useZoom=1;
 }
 mNum++;
 return TRUE;
}



BOOL PVideoInputDevice_ScreenVideo::Open(const PString & devName, BOOL /*startImmediate*/)
{
 deviceName = devName;
 dName = devName;
 mNum=0;
 h_screendc=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
 EnumDisplayMonitors(h_screendc,NULL,MyMonitorEnumProc,0);
 h_compatdc = CreateCompatibleDC(h_screendc);
 h_zoomdc = CreateCompatibleDC(h_screendc);
 h_bmpOld=NULL; h_bmp=NULL;
 h_zbmpOld=NULL; h_zbmp=NULL;

  return TRUE;    
}


BOOL PVideoInputDevice_ScreenVideo::IsOpen() 
{
  return TRUE;
}


BOOL PVideoInputDevice_ScreenVideo::Close()
{       
 SelectObject(h_compatdc, h_bmpOld);
 DeleteObject(h_bmp);
 if(useZoom==1)
 {
  SelectObject(h_zoomdc, h_zbmpOld);
  DeleteObject(h_zbmp);
 }
 DeleteDC(h_zoomdc);
 DeleteDC(h_compatdc);
 DeleteDC(h_screendc);
 if(tempFrame) { free(tempFrame); tempFrame=NULL; }
  return TRUE;
}


BOOL PVideoInputDevice_ScreenVideo::Start()
{
  return TRUE;
}


BOOL PVideoInputDevice_ScreenVideo::Stop()
{
  return TRUE;
}


BOOL PVideoInputDevice_ScreenVideo::IsCapturing()
{
  return IsOpen();
}


PStringList PVideoInputDevice_ScreenVideo::GetInputDeviceNames()
{
 int mn=GetSystemMetrics(SM_CMONITORS);

  PStringList list;
  PString mname;
 for(int i=0;i<mn;i++)
 {
  mname=PString("monitor")+PString(i);
  list.AppendString(mname);
  mname=PString("zmonitor")+PString(i);
  list.AppendString(mname);
 }
  return list;
}


BOOL PVideoInputDevice_ScreenVideo::SetVideoFormat(VideoFormat newFormat)
{
  return PVideoDevice::SetVideoFormat(newFormat);
}

BOOL PVideoInputDevice_ScreenVideo::SetColourFormat(const PString & newFormat)
{
  if (newFormat == "BGR32")
    bytesPerPixel = 4;
//  else if (newFormat == "RGB24" || newFormat == "BGR24")
//    bytesPerPixel = 3;
//  else if (newFormat *= "YUV420P")
//    bytesPerPixel = 2;
  else
    return FALSE;

  if (!PVideoDevice::SetColourFormat(newFormat))
    return FALSE;

  return SetFrameSize(frameWidth, frameHeight);
}


BOOL PVideoInputDevice_ScreenVideo::SetFrameRate(unsigned rate)
{
  if (rate < 1)
    rate = 1;
  else if (rate > 50)
    rate = 50;

  return PVideoDevice::SetFrameRate(rate);
}


BOOL PVideoInputDevice_ScreenVideo::GetFrameSizeLimits(unsigned & minWidth,
                                           unsigned & minHeight,
                                           unsigned & maxWidth,
                                           unsigned & maxHeight) 
{
  minWidth  = 16;
  minHeight = 12;
  maxWidth  = 2048;
  maxHeight =  2048;

  return TRUE;
}


BOOL PVideoInputDevice_ScreenVideo::SetFrameSize(unsigned width, unsigned height)
{
  if (!PVideoDevice::SetFrameSize(width, height))
    return FALSE;

  videoFrameSize = CalculateFrameBytes(frameWidth, frameHeight, colourFormat);
  scanLineWidth = videoFrameSize/frameHeight;

 if(h_bmp!=NULL)
 {
  SelectObject(h_compatdc, h_bmpOld);
  DeleteObject(h_bmp);
 }
 if(h_zbmp!=NULL)
 {
  SelectObject(h_zoomdc, h_zbmpOld);
  DeleteObject(h_zbmp);
 }
 if(useZoom==0)
 {
  h_bmp = CreateCompatibleBitmap(h_screendc, frameWidth, frameHeight);
  h_bmpOld = (HBITMAP)SelectObject(h_compatdc, h_bmp);
 }
 else
 {
  h_bmp = CreateCompatibleBitmap(h_screendc, screenWidth, screenHeight);
  h_bmpOld = (HBITMAP)SelectObject(h_compatdc, h_bmp);
  h_zbmp = CreateCompatibleBitmap(h_screendc, frameWidth, frameHeight);
  h_zbmpOld = (HBITMAP)SelectObject(h_zoomdc, h_zbmp);
  SetStretchBltMode(h_zoomdc, HALFTONE);
 }
 if(tempFrame) { free(tempFrame); tempFrame=NULL; }
 tempFrame=(char *)malloc(videoFrameSize);

 bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
 bmi.bmiHeader.biWidth = frameWidth;
 bmi.bmiHeader.biHeight = -(int)frameHeight;
 bmi.bmiHeader.biPlanes = 1;
 bmi.bmiHeader.biBitCount = 32;
 bmi.bmiHeader.biCompression = BI_RGB;
 bmi.bmiHeader.biXPelsPerMeter = 0;
 bmi.bmiHeader.biYPelsPerMeter = 0;
 bmi.bmiHeader.biClrImportant = 0;
 bmi.bmiHeader.biClrUsed = 0;
 bmi.bmiHeader.biSizeImage = videoFrameSize;

 return videoFrameSize > 0;
}


PINDEX PVideoInputDevice_ScreenVideo::GetMaxFrameBytes()
{
  return GetMaxFrameBytesConverted(videoFrameSize);
}


BOOL PVideoInputDevice_ScreenVideo::GetFrameData(BYTE * buffer, PINDEX * bytesReturned)
{    
  m_Pacing.Delay(1000/GetFrameRate());
  return GetFrameDataNoDelay(buffer, bytesReturned);
}

 
BOOL PVideoInputDevice_ScreenVideo::GetFrameDataNoDelay(BYTE *destFrame, PINDEX * bytesReturned)
{
  grabCount++;

  GrabScreenDCImage((BYTE *)tempFrame);

  if (NULL != converter) {
    if (!converter->Convert((BYTE *)tempFrame, destFrame, videoFrameSize, bytesReturned))
      return FALSE;
  }

  if (bytesReturned != NULL)
    *bytesReturned = videoFrameSize;

  return TRUE;
}

void PVideoInputDevice_ScreenVideo::GrabScreenDCImage(BYTE *resFrame)
{
 CURSORINFO ci;
 ICONINFO ii;
 ci.cbSize=sizeof(CURSORINFO);
 GetCursorInfo(&ci);
 GetIconInfo(ci.hCursor,&ii);
////HICON hicon = CopyIcon(ci.hCursor);
 long leftpos=screenLeft+((screenWidth-frameWidth)>>1);
 long toppos=screenTop+((screenHeight-frameHeight)>>1);
 if(useZoom==0)
 {
  StretchBlt(h_compatdc, 
               0,0, 
               frameWidth, frameHeight, 
               h_screendc, 
               leftpos, toppos,
               frameWidth, frameHeight,
               SRCCOPY);
 ////GetObject(h_bmp,sizeof(BITMAP),&bmp);
  DrawIcon(h_compatdc, ci.ptScreenPos.x-ii.xHotspot-leftpos, ci.ptScreenPos.y-ii.yHotspot-toppos, ci.hCursor);
  GetDIBits(h_compatdc, h_bmp, 0, (UINT)frameHeight, resFrame,
        &bmi, DIB_RGB_COLORS);
 }
 else
 {
  long width,height;
  if(frameWidth*screenHeight>frameHeight*screenWidth)
  {
   width=screenWidth*frameHeight/(1.0*screenHeight);
   height=frameHeight;
   toppos=0; leftpos=(frameWidth-width)>>1;
  }     
  else
  {
   height=screenHeight*frameWidth/(1.0*screenWidth);
   width=frameWidth;
   toppos=(frameHeight-height)>>1; leftpos=0;
  }
  BitBlt(h_compatdc, 
               0,0, 
               screenWidth, screenHeight, 
               h_screendc, 
               0, 0,
               SRCCOPY);
   ////GetObject(h_bmp,sizeof(BITMAP),&bmp);
  DrawIcon(h_compatdc, ci.ptScreenPos.x-ii.xHotspot-screenLeft, ci.ptScreenPos.y-ii.yHotspot-screenTop, ci.hCursor);
  StretchBlt(h_zoomdc,leftpos,toppos,width,height, 
               h_compatdc,        
               screenLeft, screenTop,
               screenWidth, screenHeight,
               SRCCOPY);
  GetDIBits(h_zoomdc, h_zbmp, 0, (UINT)frameHeight, resFrame,
        &bmi, DIB_RGB_COLORS);    
 }
 if(ii.hbmMask!=NULL) DeleteObject(ii.hbmMask);
 if(ii.hbmColor!=NULL) DeleteObject(ii.hbmColor);
//// DestroyIcon(hicon);            
}

#endif // P_VIDEO