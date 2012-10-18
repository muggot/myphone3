///////////////////////////////////////////////////////////////////////////////
// MyPhone.h : main header file for the MFC application
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

#if !defined(AFX_MYPHONE_H__A1FF011A_641B_44F1_8897_1615CE1DFF7A__INCLUDED_)
#define AFX_MYPHONE_H__A1FF011A_641B_44F1_8897_1615CE1DFF7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include <ptlib.h>
#include <h323pluginmgr.h>
#include <ptlib/pprocess.h>
#include <ptlib/sound.h>

//#include "Multilang.h"		// Languages
/////////////////////////////////////////////////////////////////////////////
// Some Globals:
	const char strProgName[]="MyPhone3";
    const int progMajorVersion = 0;
    const int progMinorVersion = 12;
	#define MF_RELEASE	//*** Uncomment this when go out of "beta" versioning ***
	const int progBuildVersion = 0;
	const char strProgPostfix[]="";
//.	const char strProgDescription[]="Voice&Video IP Telephony Client based on H323plus (www.h323plus.org)";
	const char strProgAuthor[]="H323plus project (support@h323plus.org)";
	const char strProgWeb[]="http://h323plus.org/";
	const char strProgWebHelp[]="http://h323plus.org/myphone.html";
	//
	const char strProgRespects[]="\t~~~ Thanks goes to: ~~~\r\n\
* All OpenH323/H323plus developers amd mailing list members\r\n\
* Franz J Ehrengruber (for testing and hardware support)\r\n\
* Guilhem Tardy (for help with the initial H.263 video codec)\r\n\
* Simon Horne (for incorporating in h323plus project)\r\n";

/////////////////////////////////////////////////////////////////////////////
// CMyPhoneApp:
// See MyPhone.cpp for the implementation of this class
//

////////////////////////////////////////////////////////////////////////////////////

// All this silly stuff to get the plugins to load 
// because windows is stoopid and the pluginloader never gets instanced.
// This is required for ALL MFC based applications looking to load plugins!

class PluginLoaderStartup2 : public PProcessStartup
{
  PCLASSINFO(PluginLoaderStartup2, PProcessStartup);
  public:
    void OnStartup()
    { 
      // load the actual DLLs, which will also load the system plugins
      PStringArray dirs = PPluginManager::GetPluginDirs();
      PPluginManager & mgr = PPluginManager::GetPluginManager();
      PINDEX i;
      for (i = 0; i < dirs.GetSize(); i++) 
        mgr.LoadPluginDirectory(dirs[i]);

      // now load the plugin module managers
      PFactory<PPluginModuleManager>::KeyList_T keyList = PFactory<PPluginModuleManager>::GetKeyList();
      PFactory<PPluginModuleManager>::KeyList_T::const_iterator r;
      for (r = keyList.begin(); r != keyList.end(); ++r) {
        PPluginModuleManager * mgr = PFactory<PPluginModuleManager>::CreateInstance(*r);
        if (mgr == NULL) {
          PTRACE(1, "PLUGIN\tCannot create manager for plugins of type " << *r);
        } else {
          PTRACE(3, "PLUGIN\tCreated manager for plugins of type " << *r);
          managers.push_back(mgr);
        }
      }
    }

    void OnShutdown()
    {
      while (managers.begin() != managers.end()) {
        std::vector<PPluginModuleManager *>::iterator r = managers.begin();
        PPluginModuleManager * mgr = *r;
        managers.erase(r);
        mgr->OnShutdown();
      }
    }

  protected:
    std::vector<PPluginModuleManager *> managers;
};


class CMyPhoneApp : public CWinApp
{
public:
	CMyPhoneApp();

/**** H323plus Start ****/
        class PWLibProcess : public PProcess {
          PCLASSINFO(PWLibProcess, PProcess);
#ifdef MF_RELEASE
		  PWLibProcess():PProcess("MyPhone3", strProgName, progMajorVersion, progMinorVersion, ReleaseCode, progBuildVersion) { }
#else
		  PWLibProcess():PProcess("MyPhone3", strProgName, progMajorVersion, progMinorVersion, BetaCode, progBuildVersion) { }
#endif
          void Main() { }
         
		  // This is to get the plugins to load in MFC applications
		  static void LoadPluginMgr() { plugmgr = new H323PluginCodecManager(); }
		  static void RemovePluginMgr() { delete plugmgr; }
          static H323PluginCodecManager * plugmgr;
		  static PluginLoaderStartup2 pluginLoader;
        } pwlibProcess;
/**** H323plus End ****/

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPhoneApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyPhoneApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPHONE_H__A1FF011A_641B_44F1_8897_1615CE1DFF7A__INCLUDED_)
