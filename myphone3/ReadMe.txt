*****     INFO     *****
MyPhone  - Voice&Video IP telephony client.
It is H323 protocol compatible, based on H323plus project.
Client is written in VC++, using MFC/PWLib libraries, 
compatible with any reasonable M$Windows version. 

Home Page: http://myphone.sourceforge.net/

Copyright (c) 2003 Michael Gerdov aka -=MaGGuS=-
Copyright (c) 2007 ISVO (Asia) Pte Ltd


*****     LICENSE     *****
The contents of this file are subject to the Mozilla Public License
Version 1.0 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
the License for the specific language governing rights and limitations
under the License.

*****     BUILD INSTRUCTIONS     *****
First things you need to build my project are properly compiled 
PWLib and H323plus libraries from H323plus project.
Look at www.h323plus.org for detailed instructions.
 
Thus after you completed all that, before building MyPhone you should have 
libraries headers in Include Path in your VC++ IDE and libraries themselves 
in Libraries Path.
Then you just open *.sln file from MyPhone dir and compile without a glitch... 
...theoretically (hehe :-)).

This version is only tested with VS 2003. 

*****     Debugging   *****

You can enable debugging by adding a commandline switch -{no of T's for debug level) 
ie myphone.exe -tttt will start myphone with debug tracelevel 4.
All traces are outputed to myphone_trc.txt.


*****     REGARDS     *****
Thanks goes to All OpenH323/H323plus mailing list members (www.h323plus.org)
and especially to Yuriy Gorvitovskiy for PocketBone sources (unknowingly ;-}).


GOOD LUCK!

-=MaGGuS=- (maggus@mail.ru)					2003	
Updated
Simon Horne  (s.horne@packetizer.com)				2008										