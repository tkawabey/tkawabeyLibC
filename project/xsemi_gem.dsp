# Microsoft Developer Studio Project File - Name="xsemi_gem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xsemi_gem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xsemi_gem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xsemi_gem.mak" CFG="xsemi_gem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xsemi_gem - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xsemi_gem - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xsemi_gem - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /I "../include" /I "../../sqlite" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/xsemi_gem.lib"

!ELSEIF  "$(CFG)" == "xsemi_gem - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../../sqlite" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/xsemi_gemD.lib"

!ENDIF 

# Begin Target

# Name "xsemi_gem - Win32 Release"
# Name "xsemi_gem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_ackcode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_alarm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_ceid.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_api.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_const.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_const_obj.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_impl_s1.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_impl_s2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_impl_s5.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_impl_s6.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_sqlite_if.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_gem_xxx_vvalue.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_rpt.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_spool.cpp
# End Source File
# Begin Source File

SOURCE=..\src\semi\gem\xsemi_v.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_ackcode.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_alarm.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_ceid.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_gem.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_gem_api.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_gem_const.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_gem_const_obj.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_gem_def.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_gem_sqlite_if.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_rpt.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_spool.h
# End Source File
# Begin Source File

SOURCE=..\include\xlib\semi\gem\xsemi_v.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\semi\gem\gem_db.sql
# End Source File
# End Target
# End Project
