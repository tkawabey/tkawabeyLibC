# Microsoft Developer Studio Project File - Name="xlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xlib - Win32 DebugDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xlib.mak" CFG="xlib - Win32 DebugDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "xlib - Win32 ReleaseU" (based on "Win32 (x86) Static Library")
!MESSAGE "xlib - Win32 DebugU" (based on "Win32 (x86) Static Library")
!MESSAGE "xlib - Win32 ReleaseDLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xlib - Win32 DebugDLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "xlib - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /I "../include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/xlib.lib"

!ELSEIF  "$(CFG)" == "xlib - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/xlibD.lib"

!ELSEIF  "$(CFG)" == "xlib - Win32 ReleaseU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "ReleaseU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/xlib.lib"
# ADD LIB32 /nologo /out:"../lib/xlibU.lib"

!ELSEIF  "$(CFG)" == "xlib - Win32 DebugU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/xlibD.lib"
# ADD LIB32 /nologo /out:"../lib/xlibUD.lib"

!ELSEIF  "$(CFG)" == "xlib - Win32 ReleaseDLL"

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
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../include" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_USRDLL" /D "XLIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XLIB_DLL" /D "XLIB_EXPORTS" /FR /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../lib/xlibDLL.dll" /implib:"../lib/xlibDLL.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "xlib - Win32 DebugDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSS_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XLIB_DLL" /D "XLIB_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../lib/xlibDll_d.dll" /implib:"../lib/xlibDll_d.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "xlib - Win32 Release"
# Name "xlib - Win32 Debug"
# Name "xlib - Win32 ReleaseU"
# Name "xlib - Win32 DebugU"
# Name "xlib - Win32 ReleaseDLL"
# Name "xlib - Win32 DebugDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\xarray.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xcompressbuff.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xcryptbuff.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xdeamon.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xdllorder.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xeception.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xfile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xfunctions.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xio.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xlock.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmapfile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmsgqueue.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xprocess.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xserialport.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xsocket.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xstrcordeconv.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xthread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xtime.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\xlib\xarray.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xcompressbuff.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xcryptbuff.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xdeamon.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xdllorder.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xeception.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xfile.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xfunctions.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xio.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xlock.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xmap.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xmapfile.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xmlhelper.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xmsgqueue.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xprocess.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xserialport.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xsocket.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xstrcordeconv.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xthread.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xtime.h
# End Source File
# End Group
# Begin Group "xdata"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\xdata.cpp
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xdata.h
# End Source File
# Begin Source File

SOURCE=..\src\xdata_rich.cpp
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xdata_rich.h
# End Source File
# Begin Source File

SOURCE=..\src\xdata_tbl.cpp
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xdata_tbl.h
# End Source File
# End Group
# Begin Group "buffer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\xbuff.cpp
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xbuff.h
# End Source File
# Begin Source File

SOURCE=..\src\xstrbuff.cpp
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xstrbuff.h
# End Source File
# End Group
# Begin Group "dll_define"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\xlib\xlib_head.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\xlib_tail.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\Makefile.cygwin
# End Source File
# Begin Source File

SOURCE=..\include\newxlib.xml
# End Source File
# Begin Source File

SOURCE=.\README.TXT
# End Source File
# End Target
# End Project
