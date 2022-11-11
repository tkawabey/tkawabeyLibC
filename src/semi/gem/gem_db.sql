drop table CONFIG;
CREATE TABLE CONFIG (	ConfName varchar(50) primary key, 
			ConfData varchar(255), 
			DataType INTEGER,
			Comment varchar(255));


drop table CEID;
CREATE TABLE CEID (	CeID INTEGER primary key, 
			EventName varchar(50), 
			DisplayName varchar(255), 
			Comment varchar(255), 
			Enable TINYINT);

drop table RPTID;
CREATE TABLE RPTID (	RptID INTEGER primary key, 
			RptName varchar(255),
			Comment varchar(255), 
			Enable TINYINT);

	/*
	1. Alarms
	2. Clock
	3. Commands
	4. Communication
	5. Control State
	6. Equipment Constants
	7. Events
	8. Limits Monitoring
	9. Process Programs
	10. Spooling
	11. Text Exchange
	99. Other Variables
	*/
drop table VARIABLE_GRP;
CREATE TABLE VARIABLE_GRP (	GRPID INTEGER primary key, 
			Name		varchar(256),
			Comment		varchar(255));


drop table VID;
CREATE TABLE VID (	VID INTEGER primary key, 
			Name		varchar(50),
			DisplayName	varchar(50),
			DataUnit	varchar(50),
			DataType	INTEGER,
			Mode		INTEGER,
			InitValue	blob,
			Category	INTEGER,
			Comment		varchar(255),
			VARIABLE_GRP	INTEGER,
			VPRIVATE	TINYINT,
			GET_EX		TINYINT,
			Enable		TINYINT);

drop table EC;
CREATE TABLE EC (	ECID INTEGER primary key, 
			EcMax		blob,
			EcMin		blob,
			EcDef		blob,
			EcVal		blob,
			SET_EX		TINYINT,
			ECVerify	TINYINT
			);

drop table SV;
CREATE TABLE SV (	SVID INTEGER primary key, 
			SizeMax		INTEGER,
			SizeMin		INTEGER,
			LimitMax	varchar(256),
			LimitMin	varchar(256),
			CeLimits	INTEGER);


drop table DV;
CREATE TABLE DV (	DVID INTEGER primary key, 
			SizeMax		INTEGER,
			SizeMin		INTEGER);


drop table VVALS;
CREATE TABLE VVALS (	VID INTEGER primary key, 
			VValue		varchar(255),
			Comment		varchar(255),
			CeLimits	INTEGER);


drop table CEID_RPTID;
CREATE TABLE CEID_RPTID (	CeID INTEGER, 
				RptIdSeq INTEGER,
				RptID INTEGER,
				Option TINYINT,
				Comment varchar(255),
				primary key(CeID, RptIdSeq)
			);

drop table RPTID_DVID;
CREATE TABLE RPTID_DVID (	RptID INTEGER ,
			DvIdSeq INTEGER,
			DvID INTEGER,
			primary key(RptID, DvIdSeq));



drop table ALARM;
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

drop table ACKCODES;
CREATE TABLE ACKCODES (	ACKKIND INTEGER,
				ACKCODE INTEGER, 
				Comment varchar(255),
				Enable TINYINT,
				primary key(ACKKIND, ACKCODE)
			);

drop table SPOOL_SF;
CREATE TABLE SPOOL_SF (	STREAM INTEGER,
				FUNCTION INTEGER, 
				primary key(STREAM, FUNCTION)
			);


