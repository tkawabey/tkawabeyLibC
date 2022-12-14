/**
 * @file xsemi_ackcode.h
 * @brief ¼±Ì»¢SEMIKi ACKR[hf[^ÇNX
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_ACKCODE_H__
#define __XSEMI_ACKCODE_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata_rich.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"


class XLIB_API XSEMI_GemConst;

class XLIB_API XSEMI_ACKCODE;
class XLIB_API XSEMI_ACKCODEs;

DLL_EXTERN template class XLIB_API std::vector<XSEMI_ACKCODE*>;


/**	@brief	ACKR[h	*/
class XLIB_API XSEMI_ACKCODE : public XSEMI_GemConstObjectAtom	
{

public:
	enum ATOM_POS {
			ATOM_POS_ACKKIND = 0
		,	ATOM_POS_ACKCODE
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};
	enum KIND {
			KIND_ACKA = 0		// S5F14,F15,F18,S16F2,F4,F6,F12,F14,F16,F18,F24,F26,F30,S17F4,F8,F14
		,	KIND_ACKC3			// S3F6,F8,F10
		,	KIND_ACKC5			// S5F2,F4
		,	KIND_ACKC6			// S6F2,F4,F10,F12,F14
		,	KIND_ACKC7			// S7F4,F12,F14,F16,F18,F24,F32
		,	KIND_ACKC7A			// S7F27
		,	KIND_ACKC10			// S10F2,F4,F6,F10
		,	KIND_ACKC13			// S13F2,F4,F6,F8
		,	KIND_CAACK			// Carrier Action Acknowledge Code, S3F18,F20,F22,F24,F26
		,	KIND_CEPACK			// S2F50
		,	KIND_CPACK			// S2F42
		,	KIND_CSAACK			// S2F8
		,	KIND_DATAACK		// S14F22
		,	KIND_DRACK			// S2F34
		,	KIND_EAC			// S2F16
		,	KIND_ERACK			// S2F38
		,	KIND_ERRCODE		//
		,	KIND_GRANT			// S2F2,F40,S3F16,S4F26,S13F12,S14F24,S16F2,F12
		,	KIND_GRANT6			// S6F6
		,	KIND_GRANT1			// S12F6
		,	KIND_HCACK			// S2F42,F50
		,	KIND_HOHALTACK		// S4F41
		,	KIND_LIMITACK		// S2F46
		,	KIND_LRACK			// S2F36
		,	KIND_LVACK			// S2F46
		,	KIND_MAPER			// S12F19
		,	KIND_MAPFT			// S12F3,F5
		,	KIND_MDACK			// S12F8,F10,F12
		,	KIND_MIDAC			// S3F14
		,	KIND_MIDRA			// S3F12
		,	KIND_OBJACK			// S14F2,F6,F8,F10,F12,F14,F16,F18,F26,F28
		,	KIND_OFLACK			// S1F16
		,	KIND_ONLACK			// S1F18
		,	KIND_PPGNT			// S7F2,F30
		,	KIND_RAC			// S2F20
		,	KIND_RMACK			// S15F4,F6,F8,F10,F12,F14.....
		,	KIND_RMGRANT		// S15F2,F37,F46
		,	KIND_RRACK			// S4F18
		,	KIND_RSACK			// S4F2
		,	KIND_RSDA			// S6F24
		,	KIND_RSPACK			// S2F44
		,	KIND_SDACK			// S12F2
		,	KIND_SPAACK			// S2F4
		,	KIND_SSACK			// S18F2,F4,F6,F10,F12,F14
		,	KIND_SVACK			// S14F20
		,	KIND_TBLACK			// S13F14,F16
		,	KIND_TIAACK			// S2F24
		,	KIND_TIACK			// S2F32
		,	KIND_TRACK			// S4F20,F22,F23
		,	KIND_VLAACK			// S2F46
	};
// vpeB
	/**	@brief	RXgN^
     *
	 *	@param	ackkind	ACKÌíÞ
	 *	@param	code	R[h
	*/
	XSEMI_ACKCODE(XD_UINT4 ackkind, XD_UINT1 code);
	/**	@brief	fXgN^	*/
	virtual ~XSEMI_ACKCODE();

// property	
	/**	@brief	ACKÌíÞðæ¾µÜ·B
	 *
	 *	@return	XD_UINT4
	 */
	XD_UINT4 get_AckKind() const;

	/**	@brief	ACKR[hðæ¾µÜ·B
	 *	@return	ACKR[h
	 */
	XD_UINT1 get_AckCode() const;
	/**	@brief	ACKR[hðÝèµÜ·B
	 *
	 *	@param	val	ACKR[h
	 */
	void set_AckCode(XD_UINT1 val);

	/**	@brief	Rgðæ¾µÜ·B
	 *	@return	Rg
	 */
	const TCHAR* get_Comment();
	/**	@brief	RgðÝèµÜ·B
	 *	@param	p_comment	Rg
	 */
	void set_Comment(const TCHAR* p_comment);

	/**	@brief	Lø/³øtOðæ¾µÜ·B
	 *
	 *	@retval	true	Lø
	 *	@retval	false	³ø
	 */
	bool get_Enable() const;
	/**	@brief	Lø/³øtOðæ¾µÜ·B
	 *
	 *	@param	val	true:Lø/false:³ø
	 */
	void set_Enable(bool val);

// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4					m_ulAckKind;	// Ack Kind
	XD_UINT1					m_ulAckCode;	// ACKCODE
	XStrBuff					m_strComment;	// Comment
	bool						m_bEnable;		// Lø/³ø

	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_ACKCODEs;
};



/**	@brief	ACKR[hÌêÇNX	 */
class XLIB_API XSEMI_ACKCODEs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	RXgN^ */
	XSEMI_ACKCODEs();

	/**	@brief	fXgN^ */
	virtual ~XSEMI_ACKCODEs();

	/**	@brief	CEIDÌêððúµÜ·B */
	void clear();

	/**	@brief	êðæ¾µÜ·
	 *	@return	ê
	 */
	XD_UINT4 count();

	/**	@brief	wèµ½CfbNXÌACeðæ¾µÜ·B
	 *
	 *	@param	index	CfbNX
	 *
	 *	@return	ACe
	 */
	XSEMI_ACKCODE* get(XD_UINT4 index);

	/**	@brief	wèµ½CEIDÌACeðæ¾µÜ·B
	 *
	 *	@param	id	CfbNX
	 *
	 *	@return	ACe
	 */
	XSEMI_ACKCODE* get_byID(XD_UINT4 kind, XD_UINT4 code);

	/**	@brief	êÌæªACeðæ¾·éB
	 *
	 *	@return	ACeB©Â©çÈ¢êÍNULLB
	 */
	XSEMI_ACKCODE* getFirstItem();

	/**	@brief	»ÝÌêÌJgÊuÌÌACeðæ¾·éB
	 *
	 *	@return	ACeBÅãÉBµ½êÍANULLðÔ·B
	 */
	XSEMI_ACKCODE* getNextItem();

	/**	@brief	»ÝÌêÌJgÊuÌOÌACeðæ¾·éB
	 *
	 *	@return	ACeBæªÉBµ½êÍANULLðÔ·B
	 */
	XSEMI_ACKCODE* getPrevItem();

	/**	@brief	ACeðÇÁµÜ·B
	 *
	 *	@param	pAtom	AgIuWFNg
	 *
	 *	@return	ÇÁ³ê½ACeB¸sµ½êÍNULLB
	 */
	XSEMI_ACKCODE* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	ACeðíµÜ·B
	 *
	 *	@param	pCEID	ACeðíµÜ·B
	 *
	 *	@retval	true	¬÷
	 *	@retval	false	¸s
	 */
	bool del(XSEMI_ACKCODE* pAckCode);
protected:
	std::vector<XSEMI_ACKCODE*>				m_array;
	std::vector<XSEMI_ACKCODE*>::iterator	m_ite;
	bool									m_bloaded;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_ACKCODE_H__ */
