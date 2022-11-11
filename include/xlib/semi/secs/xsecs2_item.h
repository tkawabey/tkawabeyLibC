#ifndef __XSECS_2_ITEM_H__
#define __XSECS_2_ITEM_H__

#include "xlib/xlib_head.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata.h"
#include "xlib/xdata_rich.h"
#include "xlib/xmlhelper.h"
#include "xlib/semi/secs/xsecs2.h"
/**	@brief	
 * @note Format
		<L[5]
		 1. <A[x]  name>		// name
		 2. <U4[1] type>		// type
		 3. <A comment>			// comment
		 4. <L[x]				// formats
		      <L[4]				//  format1
			    <U4[1...3] type>
			    <I4 size_min>
			    <I4 size_max>
			    <L[x]
			      <I4 cnt>		// count
			      <L or A>		// L is List/ A is item
			    >
		      >
		      <L[4]			//    format2
			    <U4[1...3] type>
			    <U4 size_min>
			    <U4 size_max>
			    <L[x]
    
			    >
		      >
		      ...
		     >
		 5. <L[x]			//	values
		      <L[2]		//  value_type
			   <U4[1...3] type>	//		type
			   <L[y]
			     <L[2]
				    < val>
				    <A "">	//			comment
			     >
			     <L[2]
   				 < val>
				    <A "">	//			comment
			     >
			     ...
			   >
		      >
		      <L[2]		//  value_type
			   <U4[1...3] type>	//		type
			   <L[y]
			     <L[2]
   				    < val>
				    <A "">	//			comment
			     >
			     <L[2]
				    < val>
				    <A "">	//			comment
			     >
			     ...
			   >
		      >
		      ...
		     >
		   >

*/
#include "xlib/xlib_tail.h"
#endif /*__XSECS_2_ITEM_H__*/
