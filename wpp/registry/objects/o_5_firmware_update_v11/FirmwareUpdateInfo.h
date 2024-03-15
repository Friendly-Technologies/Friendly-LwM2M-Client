/*
 * Generated on: 2023-11-08 13:50:54
 * Created by: SinaiR&D
 */

#ifndef WPP_FIRMWARE_UPDATE_INFO_H
#define WPP_FIRMWARE_UPDATE_INFO_H

#include "ObjectInfo.h"

#if OBJ_O_5_FIRMWARE_UPDATE_V11

namespace wpp {

inline const ObjectInfo FIRMWARE_UPDATE_OBJ_INFO = {
	/* Name */
	"Firmware Update",
	/* Object ID */
	OBJ_ID::FIRMWARE_UPDATE,
	/* URN */
	"urn:oma:lwm2m:oma:5:1.1",
	/* Object version */
	{1,1},
	/* Lwm2m version */
	{1,1},
	/* Is single */
	IS_SINGLE::SINGLE,
	/* Is Mandatory */
	IS_MANDATORY::OPTIONAL,
	/* Object supported operations */
	/* --------------- Info block 0 start --------------- */
	InstOp(	InstOp::CREATE |
			InstOp::DELETE),
	ResOp(	ResOp::READ|
			ResOp::WRITE|
			#ifdef LWM2M_RAW_BLOCK1_REQUESTS
			ResOp::BLOCK_WRITE|
			#endif
			ResOp::DISCOVER|
			ResOp::EXECUTE|
			ResOp::DELETE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_O_5_FIRMWARE_UPDATE_V11 */
#endif // WPP_FIRMWARE_UPDATE_INFO_H
