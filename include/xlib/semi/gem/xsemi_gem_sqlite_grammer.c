#ifndef __XSEMI_GEM_SQL_GRAMMER_H__
#define __XSEMI_GEM_SQL_GRAMMER_H__

//
//	Create Table •¶
//

// CONFIG
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_CONFIG = _T("CREATE TABLE CONFIG (	ConfName varchar(50) primary key,  \n")
												_T("      ConfData varchar(255),  \n")
												_T("      DataType INTEGER, \n")
												_T("      Comment varchar(255)); \n");

// CEID
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_CEID = _T("CREATE TABLE CEID (	CeID INTEGER primary key, \n")
												_T("    EventName varchar(50), \n")
												_T("    DisplayName varchar(255),  \n")
												_T("    Comment varchar(255), \n")
												_T("    Enable TINYINT);");

// RPTID
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_RPTID = _T("CREATE TABLE RPTID (	RptID INTEGER primary key,\n" )
												_T("	RptName varchar(255),\n")
												_T("	Comment varchar(255),\n")
												_T("	Enable TINYINT);");

// VARIABLE_GRP
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_VARIABLE_GRP = _T("CREATE TABLE VARIABLE_GRP (	GRPID INTEGER primary key, \n")
												_T("	Name		varchar(256),\n")
												_T("	Comment		varchar(255));");

// VID
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_VID = _T("CREATE TABLE VID (	VID INTEGER primary key, \n")
												_T("			Name		varchar(50), \n")
												_T("			DisplayName	varchar(50), \n")
												_T("			DataUnit	varchar(50), \n")
												_T("			DataType	INTEGER, \n")
												_T("			Mode		INTEGER, \n")
												_T("			InitValue	blob, \n")
												_T("			Category	INTEGER, \n")
												_T("			Comment		varchar(255), \n")
												_T("			VARIABLE_GRP	INTEGER, \n")
												_T("			VPRIVATE	TINYINT, \n")
												_T("			GET_EX		TINYINT, \n")
												_T("			Enable		TINYINT, \n")
												_T("			VValue		blob);");
// alter table VID add VValue blob;


// EC
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_EC = _T("CREATE TABLE EC (	ECID INTEGER primary key, \n")
												_T("			EcMax		blob, \n")
												_T("			EcMin		blob, \n")
												_T("			EcDef		blob, \n")
												_T("			EcVal		blob, \n")
												_T("			SET_EX		TINYINT, \n")
												_T("			ECVerify	TINYINT); ");
// SV
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_SV = _T("CREATE TABLE SV (	SVID INTEGER primary key, \n")
												_T("			SizeMax		INTEGER, \n")
												_T("			SizeMin		INTEGER, \n")
												_T("			LimitMax	blob, \n")
												_T("			LimitMin	blob, \n")
												_T("			CeLimits	INTEGER); ");
// DV
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_DV = _T("CREATE TABLE DV (	DVID INTEGER primary key, \n")
												_T("			SizeMax		INTEGER, \n")
												_T("			SizeMin		INTEGER); \n");

// VVALS
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_VVALS = _T("CREATE TABLE VVALS (	VID INTEGER,  \n")
												_T("			Vvalue		varchar(255), \n")
												_T("			Comment		varchar(255), \n")
												_T("			Enable		TINYINT, \n")
												_T("			primary key(VID, Vvalue)")
												_T(");");


// CEID_RPTID
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_CEID_RPTID = _T("CREATE TABLE CEID_RPTID (	CeID INTEGER, \n")
												_T("			RptIdSeq INTEGER,\n")
												_T("			RptID INTEGER,\n")
												_T("			Option TINYINT,\n")
												_T("			Comment varchar(255),\n")
												_T("			primary key(CeID, RptIdSeq)")
												_T(");");

// RPTID_DVID
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_RPTID_DVID = _T("CREATE TABLE RPTID_DVID (	RptID INTEGER ,\n")
												_T("			DvIdSeq INTEGER,\n")
												_T("			DvID INTEGER,\n")
												_T("			primary key(RptID, DvIdSeq));");

// ALARM
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_ALARM= _T("CREATE TABLE ALARM (	ALID INTEGER,  \n")
												_T("     ALCD INTEGER, \n")
												_T("     AlarmName varchar(255), \n")
												_T("     AlarmText varchar(255), \n")
												_T("     OnCEID INTEGER,  \n")
												_T("     OffCEID INTEGER,  \n")
												_T("     Comment varchar(255), \n")
												_T("     Enable TINYINT, \n")
												_T("     primary key(ALID) \n")
												_T(");");

// ACKCODES
const TCHAR* XSEMI_SQL_GRM_DROP_TABLE_ACKCODES  = _T("drop table ACKCODES;");
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_ACKCODES= _T("CREATE TABLE ACKCODES (	ACKKIND INTEGER,\n")
												_T("	ACKCODE INTEGER, \n")
												_T("	Comment varchar(255),\n")
												_T("	Enable TINYINT,\n")
												_T("	primary key(ACKKIND, ACKCODE)\n")
												_T(");");


// SPOOL_SF
const TCHAR* XSEMI_SQL_GRM_DROP_TABLE_SPOOL_SF  = _T("drop table SPOOL_SF;");
const TCHAR* XSEMI_SQL_GRM_CREATE_TABLE_SPOOL_SF= _T("CREATE TABLE SPOOL_SF (	STREAM INTEGER,\n")
												_T("	FUNCTION INTEGER, \n")
												_T("	primary key(STREAM, FUNCTION)\n")
												_T(");");

//
//	Select •¶
//

	/*
		CREATE TABLE CEID (	CeID INTEGER primary key, 
					EventName varchar(50), 
					Comment varchar(255), 
					Enable TINYINT);
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_CEID = _T("select * from CEID order by CeID;");
	/*
		CREATE TABLE RPTID (	RptID INTEGER primary key, 
					RptName varchar(255),
					Comment varchar(255), 
					Enable TINYINT);
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_RPTID	= _T("select * from RPTID order by RptID;");


	/*
		CREATE TABLE VARIABLE_GRP (	GRPID INTEGER primary key, 
					Name		varchar(256),
					Comment		varchar(255));
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_VARIABLE_GRP	= _T("select * from VARIABLE_GRP order by GRPID;");
const TCHAR* XSEMI_SQL_GRM_INSERT_VARIABLE_GRP	= _T("insert into  VARIABLE_GRP(GRPID, Name, Comment) values(%d, '%s', '%s');");
const TCHAR* XSEMI_SQL_GRM_UPDATE_VARIABLE_GRP	= _T("update  VARIABLE_GRP set Name='%s' \n")
													 _T("                       , Comment='%s'  \n")
													 _T(" where GRPID=%d;");
	/*
	CREATE TABLE VID (	VID INTEGER primary key, 
				Name		varchar(50),
				DisplayName	varchar(50),
				DataUnit	varchar(50),
				DataType	INTEGER,
				Mode		INTEGER,
				InitValue	varchar(256),
				Category	INTEGER,
				Comment		varchar(255),
				VARIABLE_GRP	INTEGER,
				VPRIVATE	TINYINT,
				GET_EX		TINYINT,
				Enable		TINYINT,
				VValue		blob);
	*/

	/*
		CREATE TABLE EC (	ECID INTEGER primary key, 
					EcMax		varchar(256),
					EcMin		varchar(256),
					EcDef		varchar(256),
					EcVal		varchar(256),
					SET_EX		TINYINT,
					ECVerify	TINYINT
					);
	*/
const TCHAR*  XSEMI_SQL_GRM_SELECT_EC =	_T("select		VID.VID			, VID.Name		, VID.DisplayName  \n")
									_T("		,	VID.DataUnit	, VID.DataType	, VID.Mode  \n")
									_T("		,	VID.InitValue	, VID.Category	, VID.Comment \n")
									_T("		,	VID.VARIABLE_GRP		 \n")
									_T("		,	VID.VPRIVATE		 \n")
									_T("		,	VID.GET_EX		 \n")
									_T("		,	EC.EcMax	, EC.EcMin		, EC.EcDef \n")
									_T("		,	EC.EcVal	, EC.SET_EX		, EC.ECVerify \n")
									_T("		,	VID.Enable, VID.VValue \n")
									_T("from VID,EC  \n")
									_T("where	VID.Category = 1  \n")
									_T("	and	VID.VID = EC.ECID ");
	/*
	CREATE TABLE SV (	SVID INTEGER primary key, 
				SizeMax		INTEGER,
				SizeMin		INTEGER,
				LimitMax	varchar(256),
				LimitMin	varchar(256),
				CeLimits	INTEGER);
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_SV =	_T("select		VID.VID			, VID.Name		, VID.DisplayName  \n")
									_T("		,	VID.DataUnit	, VID.DataType	, VID.Mode  \n")
									_T("		,	VID.InitValue	, VID.Category	, VID.Comment  \n")
									_T("		,	VID.VARIABLE_GRP		 \n")
									_T("		,	VID.VPRIVATE		 \n")
									_T("		,	VID.GET_EX		 \n")
									_T("		,	SV.SizeMax		, SV.SizeMin	, SV.LimitMax  \n")
									_T("		,	SV.LimitMin		, SV.CeLimits \n")
									_T("		,	VID.Enable, VID.VValue \n")
									_T("from VID,SV  \n")
									_T("where	VID.Category = 0 \n")
									_T("	and	VID.VID = SV.SVID ");
	/*
	CREATE TABLE DV (	DVID INTEGER primary key, 
				SizeMax		INTEGER,
				SizeMin		INTEGER);
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_DV =	_T("select		VID.VID			, VID.Name		, VID.DisplayName  \n")
									_T("		,	VID.DataUnit	, VID.DataType	, VID.Mode  \n")
									_T("		,	VID.InitValue	, VID.Category	, VID.Comment  \n")
									_T("		,	VID.VARIABLE_GRP		 \n")
									_T("		,	VID.VPRIVATE		 \n")
									_T("		,	VID.GET_EX		 \n")
									_T("		,	DV.SizeMax		, DV.SizeMin \n")
									_T("		,	VID.Enable, VID.VValue \n")
									_T("from VID,DV  \n")
									_T("where	VID.Category = 2 \n")
									_T("	and	VID.VID = DV.DVID ");
	/*
		CREATE TABLE CEID_RPTID (	CeID INTEGER primary key, 
					RptIdSeq INTEGER primary key,
					RptID INTEGER,
					Option TINYINT,
					Comment varchar(255),			
					Enable TINYINT,
					primary key(RptID, DvIdSeq));
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_CEID_RPTID = _T("select * from CEID_RPTID order by CeID, RptIdSeq;");
const TCHAR* XSEMI_SQL_GRM_INSERT_CEID_RPTID = _T("INSERT INTO CEID_RPTID(CeID, RptIdSeq, RptID, Option, Comment) ")
												  _T(" VALUES(%d,%d,%d,%d,'%s');");
const TCHAR* XSEMI_SQL_GRM_UPDATE_CEID_RPTID = _T("update CEID_RPTID set Option=%d, Comment='%s' where CeID=%d and RptIdSeq=%d and RptID=%d;");


	/*
		CREATE TABLE RPTID_DVID (	RptID INTEGER ,
					DvIdSeq INTEGER,
					DvID INTEGER,
					Enable TINYINT,
					primary key(RptID, DvIdSeq));
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_RPTID_DVID =	 _T("select RPTID_DVID.* \n")
											_T("from RPTID_DVID,RPTID \n")
											_T("where RPTID_DVID.RptID=RPTID.RptID \n")
											_T("order by RPTID_DVID.RptID, RPTID_DVID.DvIdSeq;");
const TCHAR* XSEMI_SQL_GRM_INSERT_RPTID_DVID =	 _T("INSERT INTO RPTID_DVID(RptID, DvIdSeq, DvID)")
													_T(" VALUES(%d,%d,%d);");
const TCHAR* XSEMI_SQL_GRM_UPDATE_RPTID_DVID =	 _T("update RPTID_DVID set DvID=%d where RptID=%d and DvIdSeq=%d;");

	/*
		CREATE TABLE ALARM (	ALID INTEGER, 
						ALCD INTEGER, 
						AlarmName varchar(255),
						AlarmText varchar(255),
						OnCEID INTEGER, 
						OffCEID INTEGER, 
						Comment varchar(255),
						Enable TINYINT,
						primary key(ALID)
					);
	*/
const TCHAR* XSEMI_SQL_GRM_SELECT_ALARM = _T("select * from ALARM order by ALID;");
const TCHAR* XSEMI_SQL_GRM_INSERT_ALARM = _T("insert into ALARM values(%d, %d, '%s', '%s', %d,  %d,  '%s', %d);");
const TCHAR* XSEMI_SQL_GRM_UPDATE_ALARM = _T("update ALARM set ALCD=%d \n")
											 _T("	,	AlarmName='%s' \n")
											 _T("	,	AlarmText='%s' \n")
											 _T("	,	OnCEID=%d \n")
											 _T("	,	OffCEID=%d \n")
											 _T("	,	Comment='%s' \n")
											 _T("	,	Enable=%d \n")
											 _T("where ALID=%d;");
const TCHAR* XSEMI_SQL_GRM_SELECT_ACKCODES = _T("select * from ACKCODES order by ACKKIND, ACKCODE;");
const TCHAR* XSEMI_SQL_GRM_SELECT_SPOOL_SF = _T("select * from SPOOL_SF order by STREAM, FUNCTION;");
const TCHAR* XSEMI_SQL_GRM_DELETE_SPOOL_SF = _T("delete from SPOOL_SF;");
#endif /* __XSEMI_GEM_SQL_GRAMMER_H__ */