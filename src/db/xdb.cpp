
#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"
#include "xlib//db/xdb.h"








xDbConnection::xDbConnection()
{

}



xDbConnection::~xDbConnection()
{

}
bool xDbConnection::beginTrans()
{
	return execute(_T("begin;"));
}
bool xDbConnection::commit()
{
	return execute(_T("commit;"));
}
bool xDbConnection::rollback()
{
	return execute(_T("rollback;"));
}



//
//	xDbStmt
//
xDbStmt::xDbStmt()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}



xDbStmt::~xDbStmt()
{

}

bool xDbStmt::next()
{
	xDbStmt::STS sts = xDbStmt::ERR;
XEXCEPT_ENTRY
	sts = this->step();
	return sts == xDbStmt::ROW;
XEXCEPT_EXIT
	return false;
}