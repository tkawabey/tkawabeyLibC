# Microsoft Developer Studio Project File - Name="xsecs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xsecs - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xsecs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xsecs.mak" CFG="xsecs - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xsecs - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xsecs - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "xsecs - Win32 DebugDLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xsecs - Win32 ReleaseDLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "xsecs - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /I "../include" /I "../../libpcap/Include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/xsecs.lib"

!ELSEIF  "$(CFG)" == "xsecs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../libpcap/Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/xsecsD.lib"

!ELSEIF  "$(CFG)" == "xsecs - Win32 DebugDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugDLL"
# PROP BASE Intermediate_Dir "DebugDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../libpcap/Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../libpcap/Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "XLIB_DLL" /D "XLIB_EXPORTS" /FR /YX /FD /GZ /c
MTL=midl.exe
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ws2_32.lib wpcap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../lib/xsecsDll_D.dll" /implib:"../lib/xsecsDll_D.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "xsecs - Win32 ReleaseDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseDLL"
# PROP BASE Intermediate_Dir "ReleaseDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDLL"
# PROP Intermediate_Dir "ReleaseDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe
# ADD BASE LIB32 /nologo /out:"../lib/xsecs.lib"
# ADD LIB32 /nologo /out:"../lib/xsecs.lib"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../../libpcap/Include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_USRDLL" /D "XLIB_DLL" /D "XLIB_EXPORTS" /YX /FD /c
MTL=midl.exe
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib wpcap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../lib/xsecsDll.dll" /implib:"../lib/xsecsDll.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "xsecs - Win32 Release"
# Name "xsecs - Win32 Debug"
# Name "xsecs - Win32 DebugDLL"
# Name "xsecs - Win32 ReleaseDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\semi\secs\xsecs.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\secs\xsecs_comm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\secs\xsecs_pcap_parser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\secs\xsecs_text_parser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\secs\xsecsHSMSPort.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\secs\xsecsSECS1Port.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\xlib\semi\secs\xsecs.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\secs\xsecs_comm.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\secs\xsecs_pcap_parser.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\secs\xsecs_text_parser.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\secs\xsecsHSMSPort.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\secs\xsecsSECS1Port.h
# End Source File
# End Group
# End Target
# End Project
