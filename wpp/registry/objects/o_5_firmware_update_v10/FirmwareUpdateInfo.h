/*
 * Generated on: 2024-03-19 13:06:08
 * Created by: SinaiR&D
 */

#ifndef WPP_FIRMWARE_UPDATE_INFO_H
#define WPP_FIRMWARE_UPDATE_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_5_FIRMWARE_UPDATE_V10

namespace wpp {

inline const ObjectInfo FIRMWARE_UPDATE_OBJ_INFO = {
	/* Name */
	"Firmware Update",
	/* Object ID */
	OBJ_ID::FIRMWARE_UPDATE,
	/* URN */
	"urn:oma:lwm2m:oma:5",
	/* Object version */
	{1,0},
	/* Lwm2m version */
	{1,0},
	/* Is single */
	IS_SINGLE::SINGLE,
	/* Is Mandatory */
	IS_MANDATORY::OPTIONAL,
	/* Object supported operations */
	/* --------------- Info block 0 start --------------- */
	InstOp(),
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

#endif /* OBJ_O_5_FIRMWARE_UPDATE_V10 */
#endif // WPP_FIRMWARE_UPDATE_INFO_H
