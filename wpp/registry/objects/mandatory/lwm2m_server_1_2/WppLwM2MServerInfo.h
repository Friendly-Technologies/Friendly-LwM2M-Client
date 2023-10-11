#ifndef WPP_LWM2M_SERVER_1_2_INFO_H
#define WPP_LWM2M_SERVER_1_2_INFO_H

#include "ObjectInfo.h"

#if OBJ_LWM2M_SERVER_1_2_M

namespace wpp {

static const ObjectInfo WPP_LWM2M_SERVER_OBJ_INFO = {
	/* Name */
	"LwM2M Server",
	/* Object ID */
	OBJ_ID::WPP_LWM2M_SERVER,
	/* URN */
	"urn:oma:lwm2m:oma:1:1.2",
	/* Object version */
	{1,2},
	/* Lwm2m version */
	{1,2},
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

#endif /* OBJ_LWM2M_SERVER_1_2_M */
#endif // WPP_LWM2M_SERVER_1_2_INFO_H
