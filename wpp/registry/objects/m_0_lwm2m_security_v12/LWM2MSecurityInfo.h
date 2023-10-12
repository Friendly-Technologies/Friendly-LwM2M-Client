#ifndef WPP_LWM2M_SECURITY_INFO_H
#define WPP_LWM2M_SECURITY_INFO_H

#include "ObjectInfo.h"

#if OBJ_M_0_LWM2M_SECURITY_V12

namespace wpp {

static const ObjectInfo WPP_LWM2M_Security_OBJ_INFO = {
	/* Name */
	"LWM2M Security",
	/* Object ID */
	OBJ_ID::LWM2M_SECURITY,
	/* URN */
	"urn:oma:lwm2m:oma:0:1.2",
	/* Object version */
	{1,2},
	/* Lwm2m version */
	{1,1},
	/* Is single */
	IS_SINGLE::MULTIPLE,
	/* Is Mandatory */
	IS_MANDATORY::MANDATORY,
	/* Object supported operations */
	/* --------------- Info block 0 start --------------- */
	InstOp(	InstOp::CREATE |
			InstOp::DELETE),
	ResOp(	ResOp::READ|
			ResOp::WRITE|
			ResOp::DISCOVER|
			ResOp::EXECUTE|
			ResOp::DELETE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_M_0_LWM2M_SECURITY_V12 */
#endif // WPP_LWM2M_SECURITY_INFO_H
