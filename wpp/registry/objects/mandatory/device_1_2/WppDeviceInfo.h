#ifndef WPP_DEVICE_1_2_INFO_H
#define WPP_DEVICE_1_2_INFO_H

#include "ObjectInfo.h"

#if OBJ_DEVICE_1_2_M

namespace wpp {

static const ObjectInfo WPP_DEVICE_OBJ_INFO = {
	/* Name */
	"Device",
	/* Object ID */
	OBJ_ID::SERVER,
	/* URN */
	"urn:oma:lwm2m:oma:3:1.2",
	/* Object version */
	{1,2},
	/* Lwm2m version */
	{1,1},
	/* Is single */
	IS_SINGLE::SINGLE,
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

#endif /* OBJ_DEVICE_1_2_M */
#endif // WPP_DEVICE_1_2_INFO_H
