/*
 * Generated on: 2023-11-02 16:43:32
 * Created by: Sinai RnD
 */

#ifndef WPP_LWM2M_SECURITY_INFO_H
#define WPP_LWM2M_SECURITY_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_M_0_LWM2M_SECURITY

namespace wpp {

inline const ObjectInfo LWM2M_SECURITY_OBJ_INFO = {
	/* Name */
	"LWM2M Security",
	/* Object ID */
	OBJ_ID::LWM2M_SECURITY,
	/* URN */
	"urn:oma:lwm2m:oma:0:1.1",
	/* Object version */
	{1,1},
	/* Lwm2m version */
	{1,1},
	/* Is single */
	IS_SINGLE::MULTIPLE,
	/* Is Mandatory */
	IS_MANDATORY::MANDATORY,
	/* Object supported operations */
	/* --------------- Info block 0 start --------------- */
	ItemOp(ItemOp::CREATE|
		   ItemOp::DELETE|
		   ItemOp::READ|
		   ItemOp::WRITE|
		   ItemOp::DISCOVER|
		   ItemOp::EXECUTE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_M_0_LWM2M_SECURITY */
#endif // WPP_LWM2M_SECURITY_INFO_H
