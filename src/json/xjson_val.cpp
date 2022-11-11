
#include "xlib/xeception.h"
#include "xlib/json/xjson_val.h"
#include "xlib/json/xjson_obj.h"
#include "xlib/json/xjson_array.h"

const XStrBuff		XJsonVal::EMPTY_STRING	= _T("");
const int			XJsonVal::EMPTY_INT = 0;
const double		XJsonVal::EMPTY_DOUBLE = 0.0;
const XJsonObj		XJsonVal::EMPTY_OBJECT = XJsonObj();
const XJsonArray	XJsonVal::EMPTY_ARRAY = XJsonArray();
const bool			XJsonVal::EMPTY_BOOL = false;




static
void XJsonVal_ERROR_REPORT(XJsonValErrorReport* pErrorRepoter, const TCHAR* pCheckStr, const TCHAR* p_msg, ...)
{
XEXCEPT_ENTRY
	if( pErrorRepoter == NULL ) {
		return ;
	}

	size_t nCharPos = 0, nLineNumber = 0,  nLinePos = 0;
	XStrBuff	strMsg(_T(""));
	va_list argList;
	va_start(argList, p_msg);
	strMsg.FormatV(p_msg, argList);
	va_end(argList);

	pErrorRepoter->getPositionInfo(pCheckStr, &nCharPos, &nLineNumber, &nLinePos);

	pErrorRepoter->error( strMsg ,nCharPos, nLineNumber, nLinePos);

XEXCEPT_EXIT
}


//------------------------------------------------------------------------------
//	XJsonValErrorReport
//------------------------------------------------------------------------------
XJsonValErrorReport::XJsonValErrorReport()
	:	m_pStart( NULL )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XJsonValErrorReport::getPositionInfo(
	  const TCHAR* pCheckStr
	, size_t* pCharPos /*= NULL*/
	, size_t *pLineNumber /*= NULL*/
	, size_t *pLineCharPos /*= NULL*/)
{
XEXCEPT_ENTRY

	if( m_pStart == NULL || pCheckStr == NULL ) {
		return ;
	}
	if( pCharPos != NULL ) *pCharPos = 1;
	if( pLineNumber != NULL ) *pLineNumber = 1;
	if( pLineCharPos != NULL ) *pLineCharPos = 1;
	
	const TCHAR* pWork = m_pStart;
	const TCHAR* pCurLine = pWork;
	size_t nCharPos = (size_t)(pCheckStr - m_pStart) / sizeof(TCHAR);
	size_t nLineNum = 1;
	size_t nLinePos = 1;

	
	while( *pWork != 0 ) {
		if( pCheckStr == pWork ) {
			break;
		}
		if( *pWork  == _T('\r') ) {
			if( *(pWork+1)  == _T('\n') ) {
				pWork++;
			}
			nLineNum++;
			pCurLine = pWork+1;
		} else
		if( *pWork  == _T('\n') ) {
			nLineNum++;
			pCurLine = pWork+1;
		}
		pWork++;
	}
	while( *pCurLine != 0 ) {
		if( pCheckStr == pCurLine ) {
			break;
		}
		nLinePos++;
		pCurLine++;
	}

	if( pCharPos != NULL ) *pCharPos = nCharPos;
	if( pLineNumber != NULL ) *pLineNumber = nLineNum;
	if( pLineCharPos != NULL ) *pLineCharPos = nLinePos;
XEXCEPT_EXIT
}



XJsonVal::XJsonVal()
	:	m_type(NULL_VALUE)
	,	m_data()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}




//------------------------------------------------------------------------------
//	XJsonVal
//------------------------------------------------------------------------------
XJsonVal::XJsonVal(const char *newCString)
	:	m_type(STRING)
	,	m_data( new XStrBuff(newCString)  )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XJsonVal::XJsonVal(const wchar_t *newCString)
	:	m_type(STRING)
	,	m_data( new XStrBuff(newCString)  )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XJsonVal::XJsonVal(int newInt)
	:	m_type(INTEGER)
	,	m_data( new int(newInt)  )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XJsonVal::XJsonVal(double newDouble)
	:	m_type(DOUBLE)
	,	m_data( new double(newDouble)  )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XJsonVal::XJsonVal(const XJsonObj &newObject)
	:	m_type(OBJECT)
	,	m_data( new XJsonObj(newObject)  )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XJsonVal::XJsonVal(const XJsonArray &newArray)
	:	m_type(ARRAY)
	,	m_data( new XJsonArray(newArray)  )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XJsonVal::XJsonVal(bool newBoolean)
	:	m_type(BOOLEAN)
	,	m_data( new bool(newBoolean)  )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XJsonVal::XJsonVal(const XJsonVal &src)
	:	m_type(src.m_type)
{
XEXCEPT_ENTRY
	switch (m_type) {
	case STRING:
		m_data.m_pStringValue = new XStrBuff(*src.m_data.m_pStringValue);
		break;

	case INTEGER:
		m_data.m_pIntValue = new int(*src.m_data.m_pIntValue);
		break;

	case DOUBLE:
		m_data.m_pDoubleValue = new double(*src.m_data.m_pDoubleValue);
		break;

	case OBJECT:
		m_data.m_pObjectValue = new XJsonObj(*src.m_data.m_pObjectValue);
		break;

	case ARRAY:
		m_data.m_pArrayValue = new XJsonArray(*src.m_data.m_pArrayValue);
		break;

	case BOOLEAN:
		m_data.m_pBoolValue = new bool(*src.m_data.m_pBoolValue);
		break;

	default:
		m_type = NULL_VALUE;
		break;
	}
XEXCEPT_EXIT
}


XJsonVal::~XJsonVal()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}


XJsonVal &XJsonVal::operator=(const XJsonVal &src)
{
XEXCEPT_ENTRY
	if (this == &src) {
		return *this;
	}
	clear();
	m_type = src.m_type;


	switch (m_type) {
	case STRING:
		m_data.m_pStringValue = new XStrBuff(*src.m_data.m_pStringValue);
		break;

	case INTEGER:
		m_data.m_pIntValue = new int(*src.m_data.m_pIntValue);
		break;

	case DOUBLE:
		m_data.m_pDoubleValue = new double(*src.m_data.m_pDoubleValue);
		break;

	case OBJECT:
		m_data.m_pObjectValue = new XJsonObj(*src.m_data.m_pObjectValue);
		break;

	case ARRAY:
		m_data.m_pArrayValue = new XJsonArray(*src.m_data.m_pArrayValue);
		break;

	case BOOLEAN:
		m_data.m_pBoolValue = new bool(*src.m_data.m_pBoolValue);
		break;
	default:
		m_type = NULL_VALUE;
		m_data.m_pStringValue = NULL;
		break;
	}

XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator=(const char *src)
{
XEXCEPT_ENTRY
	this->setString(src);
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator=(const wchar_t *src)
{
XEXCEPT_ENTRY
	this->setString(src);
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator=(int src)
{
XEXCEPT_ENTRY
	this->setInteger(src);
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator=(double src)
{
XEXCEPT_ENTRY
	this->setDouble(src);
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator=(const XJsonObj &src)
{
XEXCEPT_ENTRY
	this->setObject(src);
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator=(const XJsonArray &src)
{
XEXCEPT_ENTRY
	this->setArray(src);
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator=(bool src)
{
XEXCEPT_ENTRY
	this->setBoolean(src);
XEXCEPT_EXIT
	return *this;
}


bool XJsonVal::operator==(const XJsonVal &rhs) const
{
	bool result = true;
XEXCEPT_ENTRY
	if (this == &rhs) {
		return result;
	}
	if (m_type == rhs.m_type) {
		switch (m_type) {
		case STRING:
			result = XStrBuff::StrCmp((*m_data.m_pStringValue).c_str(), rhs.getString().c_str()) == 0;
			break;

		case INTEGER:
			result = (*m_data.m_pIntValue == rhs.getInteger());
			break;

		case DOUBLE:
			result = (*m_data.m_pDoubleValue == rhs.getDouble());
			break;

		case OBJECT:
			result = (*m_data.m_pObjectValue == rhs.getObject());
			break;

		case ARRAY:
			result = (*m_data.m_pArrayValue == rhs.getArray());
			break;

		case BOOLEAN:
			result = (*m_data.m_pBoolValue == rhs.getBoolean());
			break;

		default:
			break;
		}

	} else {
		result = false;
	}

XEXCEPT_EXIT
	return result;
}

bool XJsonVal::operator!=(const XJsonVal &rhs) const
{
XEXCEPT_ENTRY
	return !(*this == rhs);
XEXCEPT_EXIT
	return true;
}

bool XJsonVal::operator <(const XJsonVal &rhs) const
{
	bool result = false;
XEXCEPT_ENTRY
	if (m_type == rhs.m_type) {
		switch (m_type) {
		case STRING:
			result = XStrBuff::StrCmp((*m_data.m_pStringValue).c_str(), rhs.getString().c_str()) < 0;
			break;

		case INTEGER:
			result = (*m_data.m_pIntValue < rhs.getInteger());
			break;

		case DOUBLE:
			result = (*m_data.m_pDoubleValue < rhs.getDouble());
			break;

		case OBJECT:
			result = (*m_data.m_pObjectValue < rhs.getObject());
			break;

		case ARRAY:
			result = (*m_data.m_pArrayValue < rhs.getArray());
			break;

		case BOOLEAN:
			result = (*m_data.m_pBoolValue < rhs.getBoolean());
			break;

		default:
			break;
		}

	} else {
		result = false;
	}
XEXCEPT_EXIT
	return result;
}

bool XJsonVal::operator <=(const XJsonVal &rhs) const
{
XEXCEPT_ENTRY
	return *this < rhs || *this == rhs;
XEXCEPT_EXIT
	return false;
}

bool XJsonVal::operator >(const XJsonVal &rhs) const
{
XEXCEPT_ENTRY
	return !(*this <= rhs);
XEXCEPT_EXIT
	return false;
}

bool XJsonVal::operator >=(const XJsonVal &rhs) const
{
XEXCEPT_ENTRY
	return *this > rhs || *this == rhs;
XEXCEPT_EXIT
	return false;
}


XJsonVal &XJsonVal::operator[](const char *key)
{
XEXCEPT_ENTRY
	if (m_type != OBJECT) {
		clear();
		m_type = OBJECT;
		m_data.m_pObjectValue = new XJsonObj();
	}

	return (*m_data.m_pObjectValue)[key];
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator[](const wchar_t *key)
{
XEXCEPT_ENTRY
	if (m_type != OBJECT) {
		clear();
		m_type = OBJECT;
		m_data.m_pObjectValue = new XJsonObj();
	}

	return (*m_data.m_pObjectValue)[key];
XEXCEPT_EXIT
	return *this;
}


XJsonVal &XJsonVal::operator[](size_t index)
{
XEXCEPT_ENTRY
	if (m_type != ARRAY) {
		clear();
		m_type = ARRAY;
		m_data.m_pArrayValue = new XJsonArray();
	} else if (index >= (*m_data.m_pArrayValue).size()) {
		// We make sure the array is big enough.
		m_data.m_pArrayValue->resize(index + 1);
	}

	return (*m_data.m_pArrayValue)[index];
XEXCEPT_EXIT
	return *this;
}


XJsonVal::TYPE XJsonVal::getType() const 
{
	return m_type;
}


bool XJsonVal::isString() const {
	return m_type == STRING;
}


bool XJsonVal::isStringable() const {
	return m_type != ARRAY && m_type != OBJECT;
}


bool XJsonVal::isInteger() const {
	return m_type == INTEGER;
}


bool XJsonVal::isDouble() const {
	return m_type == DOUBLE;
}


bool XJsonVal::isNumeric() const {
	return m_type == INTEGER || m_type == DOUBLE;
}


bool XJsonVal::isObject() const {
	return m_type == OBJECT;
}


bool XJsonVal::isArray() const {
	return m_type == ARRAY;
}


bool XJsonVal::isBoolean() const {
	return m_type == BOOLEAN;
}


bool XJsonVal::isNull() const {
	return m_type == NULL_VALUE;
}


const XStrBuff &XJsonVal::getString() const {
	return tryGetString(EMPTY_STRING);
}

const XStrBuff &XJsonVal::tryGetString(const XStrBuff &defaultValue) const {
	return (m_type == STRING) ? (*m_data.m_pStringValue) : (defaultValue);
}


const XStrBuff XJsonVal::getToString() const 
{
XEXCEPT_ENTRY
	if (m_type == STRING) {
		return  *m_data.m_pStringValue;
	} else {
		XStrBuff retStr = _T("");
		switch (m_type) {
		case INTEGER: 
			retStr << *m_data.m_pIntValue;
			return retStr;
		case DOUBLE:
			retStr << *m_data.m_pDoubleValue;
			return retStr;
		case BOOLEAN:
			return (*m_data.m_pBoolValue) ? _T("true") : _T("false");
		case NULL_VALUE:
			return _T("null");
		default:
			return retStr;
		}
	}
XEXCEPT_EXIT
	return _T("");
}

void XJsonVal::setString(const char* newString) 
{
XEXCEPT_ENTRY
	if (m_type == STRING) {
		*m_data.m_pStringValue = newString;

	} else {
		clear();
		m_type = STRING;
		m_data.m_pStringValue = new XStrBuff(newString);
	}
XEXCEPT_EXIT
}

void XJsonVal::setString(const wchar_t* newString) 
{
XEXCEPT_ENTRY
	if (m_type == STRING) {
		*m_data.m_pStringValue = newString;

	} else {
		clear();
		m_type = STRING;
		m_data.m_pStringValue = new XStrBuff(newString);
	}
XEXCEPT_EXIT
}



int XJsonVal::getInteger() const
{
XEXCEPT_ENTRY
	return tryGetInteger(EMPTY_INT);
XEXCEPT_EXIT
	return EMPTY_INT;
}

int XJsonVal::tryGetInteger(int defaultValue) const 
{
XEXCEPT_ENTRY
	return (m_type == INTEGER) ? (*m_data.m_pIntValue) : ((m_type == DOUBLE) ? (static_cast<int>(*m_data.m_pDoubleValue)) : (defaultValue));
XEXCEPT_EXIT
	return EMPTY_INT;
}

void XJsonVal::setInteger(int newInteger)
{
XEXCEPT_ENTRY
	if (m_type == INTEGER) {
		*m_data.m_pIntValue = newInteger;

	} else {
		clear();
		m_type = INTEGER;
		m_data.m_pIntValue = new int(newInteger);
	}
XEXCEPT_EXIT
}

double XJsonVal::getDouble() const
{
XEXCEPT_ENTRY
	return tryGetDouble(EMPTY_DOUBLE);
XEXCEPT_EXIT
	return EMPTY_DOUBLE;
}

double XJsonVal::tryGetDouble(double defaultValue) const
{
XEXCEPT_ENTRY
	return (m_type == DOUBLE) ? (*m_data.m_pDoubleValue) : ((m_type == INTEGER) ? (static_cast<double>(*m_data.m_pIntValue)) : (defaultValue));
XEXCEPT_EXIT
	return EMPTY_DOUBLE;
}

float XJsonVal::getFloat() const
{
XEXCEPT_ENTRY
	return tryGetFloat(static_cast<float>(EMPTY_DOUBLE));
XEXCEPT_EXIT
	return static_cast<float>(EMPTY_DOUBLE);
}

float XJsonVal::tryGetFloat(float defaultValue) const
{
XEXCEPT_ENTRY
	return (m_type == DOUBLE) ? (static_cast<float>(*m_data.m_pDoubleValue)) : ((m_type == INTEGER) ? (static_cast<float>(*m_data.m_pIntValue)) : (defaultValue));
XEXCEPT_EXIT
	return static_cast<float>(EMPTY_DOUBLE);
}

void XJsonVal::setDouble(double newDouble)
{
XEXCEPT_ENTRY
	if (m_type == DOUBLE) {
		*m_data.m_pDoubleValue = newDouble;

	} else {
		clear();
		m_type = DOUBLE;
		m_data.m_pDoubleValue = new double(newDouble);
	}
XEXCEPT_EXIT
}

const XJsonObj &XJsonVal::getObject() const
{
XEXCEPT_ENTRY
	return (m_type == OBJECT) ? (*m_data.m_pObjectValue) : (EMPTY_OBJECT);
XEXCEPT_EXIT
	return EMPTY_OBJECT;
}

void XJsonVal::setObject(const XJsonObj &newObject)
{
XEXCEPT_ENTRY
	if (m_type == OBJECT) {
		*m_data.m_pObjectValue = newObject;

	} else {
		clear();
		m_type = OBJECT;
		m_data.m_pObjectValue = new XJsonObj(newObject);
	}
XEXCEPT_EXIT
}


const XJsonArray &XJsonVal::getArray() const
{
XEXCEPT_ENTRY
	return (m_type == ARRAY) ? (*m_data.m_pArrayValue) : (EMPTY_ARRAY);
XEXCEPT_EXIT
	return EMPTY_ARRAY;
}

void XJsonVal::setArray(const XJsonArray &newArray)
{
XEXCEPT_ENTRY
	if (m_type == ARRAY) {
		*m_data.m_pArrayValue = newArray;

	} else {
		clear();
		m_type = ARRAY;
		m_data.m_pArrayValue = new XJsonArray(newArray);
	}
XEXCEPT_EXIT
}

bool XJsonVal::getBoolean() const
{
XEXCEPT_ENTRY
	return tryGetBoolean(EMPTY_BOOL);
XEXCEPT_EXIT
	return EMPTY_BOOL;
}

bool XJsonVal::tryGetBoolean(bool defaultValue) const
{
XEXCEPT_ENTRY
	return (m_type == BOOLEAN) ? (*m_data.m_pBoolValue) : (EMPTY_BOOL);
XEXCEPT_EXIT
	return EMPTY_BOOL;
}

void XJsonVal::setBoolean(bool newBoolean)
{
XEXCEPT_ENTRY
	if (m_type == BOOLEAN) {
		*m_data.m_pBoolValue = newBoolean;

	} else {
		clear();
		m_type = BOOLEAN;
		m_data.m_pBoolValue = new bool(newBoolean);
	}
XEXCEPT_EXIT
}

void XJsonVal::setNull()
{
XEXCEPT_ENTRY
	clear();
	m_type = NULL_VALUE;
	m_data.m_pStringValue = NULL;
XEXCEPT_EXIT
}

const TCHAR* XJsonVal::loadFromString(const TCHAR* json, XJsonValErrorReport* pErrorRepoter /*= NULL*/)
{
XEXCEPT_ENTRY
	if( pErrorRepoter != NULL ) {
		pErrorRepoter->setStartJsonStrings( json );
	}
	
	while( *json != 0 ) {
		if( *json == _T('\"') ) {
			json++;
			this->setString( _T("") );
			json = this->readString(json, *m_data.m_pStringValue, pErrorRepoter);
			return json;
		} else
		if( *json == _T('{') ) {
			json++;
			this->setObject( XJsonObj() );
			json = this->readObject(json, *m_data.m_pObjectValue, pErrorRepoter);
			return json;
		} else
		if( *json == _T('[') ) {
			json++;
			this->setArray( XJsonArray() );
			json = this->readArray(json, *m_data.m_pArrayValue, pErrorRepoter);
			return json;
		} else
		if( XStrBuff::StrLen(json) >= 4 && memicmp(json, _T("null"), 4*sizeof(TCHAR) ) == 0 ) {
			json += 4;
			this->setNull();
			return json;
		} else
		if( XStrBuff::StrLen(json) >= 4 && memicmp(json, _T("true"), 4*sizeof(TCHAR) ) == 0 ) {
			json += 4;
			this->setBoolean(true);
			return json;
		} else
		if( XStrBuff::StrLen(json) >= 5 && memicmp(json, _T("false"), 4*sizeof(TCHAR) ) == 0 ) {
			json += 5;
			this->setBoolean(false);
			return json;
		} else
		if( *json == _T('-') || _T('0') <= *json && *json <= _T('9') ) {
			json = this->readNumber(json, *this, pErrorRepoter);
			return json;
		} else
		if( !isWhiteSpace( *json ) ) {
			XJsonVal_ERROR_REPORT(pErrorRepoter, json, _T("invalid charactor(%c)"), *json );
			return NULL;
		}
		json++;
	}
	return json;
XEXCEPT_EXIT
	return NULL;
}

bool XJsonVal::isHexDigit(TCHAR digit)
{
XEXCEPT_ENTRY
	if( (_T('0') <= digit && digit <= _T('9')) ||
		(_T('a') <= digit && digit <= _T('f')) ||
		(_T('A') <= digit && digit <= _T('F'))) {
		return true;
	}
XEXCEPT_EXIT
	return false;
}


bool XJsonVal::isWhiteSpace(TCHAR whiteSpace)
{
XEXCEPT_ENTRY
	if( _T(' ') == whiteSpace ||
		_T('\t') == whiteSpace ||
		_T('\r') == whiteSpace ||
		_T('\n') == whiteSpace) {
		return true;
	}
XEXCEPT_EXIT
	return false;
}

const TCHAR* XJsonVal::readString(const TCHAR* p_str, XStrBuff &result, XJsonValErrorReport* pErrorRepoter /*= NULL*/)
{
XEXCEPT_ENTRY
	if( pErrorRepoter != NULL ) {
		pErrorRepoter->setStartJsonStrings( p_str );
	}
	while( *p_str != 0 ) {
		if( *p_str == _T('\\') ) {
			p_str++;
			if( *p_str == 0 ) {
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str-1, _T("invalid escape string.") );
				return p_str;
			}
			switch( *p_str  ) {
			case _T('\"'):	// QUOTATION_MARK
				result.appendChar( _T('\"') );
				break;
			case _T('\\'):	// REVERSE_SOLIDUS
				result.appendChar( _T('\\') );
				break;
			case _T('/'):	// SOLIDUS
				result.appendChar( _T('/') );
				break;
			case _T('b'):	// BACKSPACE
				result.appendChar( _T('\b') );
				break;
			case _T('f'):	// FORM_FEED
				result.appendChar( _T('\f') );
				break;
			case _T('n'):	// LINE_FEED
				result.appendChar( _T('\n') );
				break;
			case _T('r'):	// CARRIAGE_RETURN
				result.appendChar( _T('\r') );
				break;
			case _T('t'):	// TAB
				result.appendChar( _T('\t') );
				break;
			case _T('u'):	// UNICODE
				{
					TCHAR szUniNum[4] = {0};
					if( XStrBuff::StrLen(p_str) < 5 ) {
						XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid unicode charactor(\\%c). ") , *p_str);
						return NULL;
					}
					::memset(szUniNum, 0, sizeof(szUniNum));
					for(int i = 0; i < 4; i++ ) {
						p_str++;
						if( !isHexDigit(*p_str) ) {
							XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid unicode charactor(%c). ") , *p_str);
							return NULL;
						}
						szUniNum[i] = *p_str;
					}
					wchar_t c = (wchar_t)XStrBuff::strtol(szUniNum, NULL, 16);
					result.appendChar( c );
				}
				break;
			default:
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid escape string(\\%c). ") , *p_str);
				return NULL;
				break;
			}


		} else
		if( *p_str == _T('"') ) {
			p_str++;

			return p_str;
			break;
		} else {
			result.appendChar( *p_str );
		}		 
		p_str++;
	}


	XJsonVal_ERROR_REPORT(pErrorRepoter, p_str-1, _T("not terminate string."));

	return NULL;
XEXCEPT_EXIT
	return NULL;
}


const TCHAR* XJsonVal::readObject(const TCHAR* p_str, XJsonObj &result, XJsonValErrorReport* pErrorRepoter /*= NULL*/)
{
XEXCEPT_ENTRY
	if( pErrorRepoter != NULL ) {
		pErrorRepoter->setStartJsonStrings( p_str );
	}

	while( *p_str != 0 ) {
		if( *p_str == _T('"') ) {
			// Ex:
			//   "tag1" : "value"
			//   "tag2" : 1.2.3
			XStrBuff strTag(_T(""));
			TCHAR    cWk = 0;
			const TCHAR* p_savestr = NULL;

			p_str++;
			p_str = XJsonVal::readString(p_str, strTag, pErrorRepoter);
			if( p_str == NULL ) {
				return p_str;
			}
			p_savestr = p_str;
			p_str = XJsonVal::readToNonWhiteSpace(p_str, cWk);
			if( p_str == NULL ) {
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_savestr, _T("property-name dose not terminate.") );
				return p_str;
			}
			if( *p_str != _T(':')){
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid charactor(%c). ") , *p_str);
				return NULL;
			}
			p_str++;
			p_savestr = p_str;
			p_str = XJsonVal::readToNonWhiteSpace(p_str, cWk);
			if( p_str == NULL ) {
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_savestr, _T("property-value dose not terminate.") );
				return p_str;
			}

			p_str =result[(const TCHAR*)strTag].loadFromString(p_str, pErrorRepoter);
			if( p_str == NULL ) {
				return p_str;
			}

			p_savestr = p_str;
			p_str = XJsonVal::readToNonWhiteSpace(p_str, cWk);
			if( p_str == NULL ) {
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_savestr, _T("property-value dose not terminate.") );
				return p_str;
			}
			if( *p_str == _T('}') ) {
				p_str++;
				return p_str;
			} else			
			if( *p_str != _T(',') ) {
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid charactor(%c). ") , *p_str);
				return NULL;
			}
		} else
		if( *p_str == _T('}') ) {

			return p_str;
		} else if( !isWhiteSpace(*p_str) ) {
			// Error.
			XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid charactor(%c)"), *p_str );
			return NULL;
		}

		p_str++;
	}
	return p_str;
XEXCEPT_EXIT
	return NULL;
}


const TCHAR* XJsonVal::readArray(const TCHAR* p_str, XJsonArray &result, XJsonValErrorReport* pErrorRepoter /*= NULL*/)
{
XEXCEPT_ENTRY
	if( pErrorRepoter != NULL ) {
		pErrorRepoter->setStartJsonStrings( p_str );
	}

	while( *p_str != 0 ) {
		if( *p_str == _T(']') ) {
			return p_str;
		}  else 
		if (!isWhiteSpace(*p_str)) {
			const TCHAR* p_savestr = NULL;
			TCHAR    cWk = 0;

			result.push_back(XJsonVal());
			result.back().m_type = UNKNOWN;
			p_str = result.back().loadFromString(p_str);
			if( p_str == NULL ) {
				return p_str;
			}
			if (result.back().m_type == UNKNOWN) {
				result.pop_back();
			}
			
			p_savestr = p_str;
			p_str = XJsonVal::readToNonWhiteSpace(p_str, cWk);
			if( p_str == NULL ) {
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_savestr, _T("property-value dose not terminate.") );
				return p_str;
			}
			if( *p_str == _T(']') ) {
				p_str++;
				return p_str;
			} else			
			if( *p_str != _T(',') ) {
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid charactor(%c). ") , *p_str);
				return NULL;
			}
		}
		p_str++;
	}
	return p_str;
XEXCEPT_EXIT
	return NULL;
}


const TCHAR* XJsonVal::readNumber(const TCHAR* p_str, XJsonVal &result, XJsonValErrorReport* pErrorRepoter /*= NULL*/)
{
XEXCEPT_ENTRY
	bool notDone = true, inFraction = false, inExponent = false;
	XStrBuff	strNumber;
	strNumber.Alloc( 64 );

	strNumber = _T("");
	
	if( pErrorRepoter != NULL ) {
		pErrorRepoter->setStartJsonStrings( p_str );
	}


	// 0 ‚ª‚QŒÂ‘±‚­B
	if( p_str == NULL ) {
		return NULL;
	}
	if( *p_str == '0' ) {
		if( *(p_str+1) == '0' ) {
			XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid number.") );
			return NULL;
		}
	}
	while( *p_str != 0 ) {
		if( *p_str == _T('-')) {
			if( strNumber.getLength() != 0 ) {
				// ERR
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid number") );
				return NULL;
			} 
			strNumber.appendChar( *p_str );
		} else
		if( *p_str >= _T('0') && *p_str <= _T('9') ) {
			strNumber.appendChar( *p_str );
		} else
		if( *p_str == _T('.') ) {
			if (!inFraction && !inExponent) {
				inFraction = true;
				strNumber.appendChar( *p_str );
			} else {
				// ERR
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid number") );
				return NULL;
			}
		} else
		if( *p_str == _T('e') || *p_str == _T('E') ) {
			if (!inExponent) {
				inExponent = true;
				strNumber.appendChar( *p_str );

				if ( *(p_str+1) == '-' || *(p_str+1)  == '+') {
					p_str++;
					strNumber.appendChar( *p_str );
				} else {
					// ERR
					XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid number") );
					return NULL;
				}
			} else {
				// ERR
				XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid number") );
				return NULL;
			}
			
		} else {
			break;
		}
		p_str++;
	}

	if( strNumber.getLength() == 0 ) {
		// ERR
		XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid number") );
		return NULL;
	}
	if( !( isWhiteSpace( *p_str ) || 
			*p_str == _T(']') ||
			*p_str == _T('}') ||
			*p_str == _T(',') ||
			*p_str == 0 )) {
		// ERR
		XJsonVal_ERROR_REPORT(pErrorRepoter, p_str, _T("invalid charactor(%c)"), *p_str );
		return NULL;
	}

	if (inFraction || inExponent) {
		double doubleResult = XStrBuff::strtod(strNumber, NULL);
		result.setDouble(doubleResult);
	} else {
		int intResult = XStrBuff::strtol(strNumber, NULL, 10);
		result.setInteger(intResult);
	}
	return p_str;
XEXCEPT_EXIT
	return NULL;
}


const TCHAR* XJsonVal::readToNonWhiteSpace(const TCHAR* p_str,
		                        TCHAR &currentCharacter)
{
XEXCEPT_ENTRY	
	while( *p_str != 0 ) {
		if( ! isWhiteSpace( *p_str ) ) {
			break;
		}
		p_str++;
	}
	currentCharacter = *p_str;
	return p_str;
XEXCEPT_EXIT
	return NULL;
}


void XJsonVal::clear()
{
XEXCEPT_ENTRY	
	switch (m_type) {
	case STRING:
		delete m_data.m_pStringValue;
		break;

	case INTEGER:
		delete m_data.m_pIntValue;
		break;

	case DOUBLE:
		delete m_data.m_pDoubleValue;
		break;

	case OBJECT:
		delete m_data.m_pObjectValue;
		break;

	case ARRAY:
		delete m_data.m_pArrayValue;
		break;

	case BOOLEAN:
		delete m_data.m_pBoolValue;
		break;

	default:
		break;
	}
XEXCEPT_EXIT
}

void XJsonVal::printEscapeString(XLIB_STD_OUT_STR_STREAM &stream, const XStrBuff& str)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	const wchar_t* pc = str.c_str();
#else 
	XStrBuff strElBuff = str.c_str();
	const wchar_t* pc = strElBuff;
#endif
	while( *pc != 0 ) {
		if( *pc == _T('\"') ) {			// QUOTATION_MARK
			stream <<  _T("\\\"");
		} else
		if( *pc == _T('\\') ) {		// REVERSE_SOLIDUS
			stream <<  _T("\\\\");
		} else
		if( *pc == _T('\b') ) {		//BACKSPACE
			stream <<  _T("\\b");
		} else
		if( *pc == _T('\f') ) {		// FORM_FEED
			stream <<  _T("\\f");
		} else
		if( *pc == _T('\n') ) {		// LINE_FEED
			stream <<  _T("\\n");
		} else
		if( *pc == _T('\r') ) {		// CARRIAGE_RETURN
			stream <<  _T("\\r");
		} else
		if( *pc == _T('\t') ) {		// TAB
			stream <<  _T("\\t");
		}  else 
		if ( *pc > 0x00FF) {
			stream <<  _T("\\u");
			TCHAR szBuff[64] = {0};
			XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%04X"), *pc);
			stream <<  szBuff;
		} else {
			stream <<  *pc;
		}
		pc++;
	}

XEXCEPT_EXIT
}
void XJsonVal::print(XLIB_STD_OUT_STR_STREAM &stream
					 , size_t nDepth /*= 0*/
					 , bool bNonEscape /*= false*/) const
{
XEXCEPT_ENTRY	
	switch (m_type) {
		case XJsonVal::STRING:
			if( bNonEscape == false ) {
				stream << _T("\"");
				printEscapeString(stream, *m_data.m_pStringValue );
				stream << _T("\"");
			} else {
				stream << (*m_data.m_pStringValue).c_str();
			}
			break;

		case XJsonVal::INTEGER:
			stream << this->getInteger();
			break;

		case XJsonVal::DOUBLE: 
			{

				TCHAR szBuff[64] = {0};
				XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%g"), this->getDouble() );
				stream <<  szBuff;
			}
			break;

		case XJsonVal::OBJECT:
			this->getObject().print(stream, nDepth, bNonEscape);
			break;

		case XJsonVal::ARRAY:
			this->getArray().print(stream, nDepth, bNonEscape);
			break;

		case XJsonVal::BOOLEAN:
			if( this->getBoolean() ) {
				stream << _T("true");
			} else {
				stream << _T("false");
			}
			break;

		case XJsonVal::NULL_VALUE:
			stream << _T("null");
			break;

		default:
			break;
		}
XEXCEPT_EXIT
}




// -----------------------------------
// XJsonVal::ValueDataPointer
// -----------------------------------
XJsonVal::ValueDataPointer::ValueDataPointer()
	:	m_pStringValue(NULL) 
{
}


XJsonVal::ValueDataPointer::ValueDataPointer(XStrBuff *newStringValue) 
	:	m_pStringValue(newStringValue) 
{
}


XJsonVal::ValueDataPointer::ValueDataPointer(int *newIntValue) 
	:	m_pIntValue(newIntValue) 
{
}


XJsonVal::ValueDataPointer::ValueDataPointer(double *newDoubleValue) 
	:	m_pDoubleValue(newDoubleValue) 
{
}


XJsonVal::ValueDataPointer::ValueDataPointer(XJsonObj *newObjectValue) 
	:	m_pObjectValue(newObjectValue) 
{
}


XJsonVal::ValueDataPointer::ValueDataPointer(XJsonArray *newArrayValue) 
	:	m_pArrayValue(newArrayValue) 
{
}


XJsonVal::ValueDataPointer::ValueDataPointer(bool *newBoolValue) 
	:	m_pBoolValue(newBoolValue) 
{
}



