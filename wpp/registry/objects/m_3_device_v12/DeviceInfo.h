/*
 * Generated on: 2023-11-02 16:38:01
 * Created by: SinaiR&D
 */

#ifndef WPP_DEVICE_INFO_H
#define WPP_DEVICE_INFO_H

#include "ObjectInfo.h"

#if OBJ_M_3_DEVICE_V12

namespace wpp {

static const ObjectInfo DEVICE_OBJ_INFO = {
	/* Name */
	"Device",
	/* Object ID */
	OBJ_ID::DEVICE,
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
	InstOp(	InstOp::CREATE),
	ResOp(	ResOp::READ|
			ResOp::WRITE|
			ResOp::DISCOVER|
			ResOp::EXECUTE|
			ResOp::DELETE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_M_3_DEVICE_V12 */
#endif // WPP_DEVICE_INFO_H
