/*
 * Generated on: 2024-04-03 15:26:42
 * Created by: Sinai RnD
 */

#ifndef WPP_FIRMWARE_UPDATE_INFO_H
#define WPP_FIRMWARE_UPDATE_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_5_FIRMWARE_UPDATE

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
	ItemOp(ItemOp::READ|
		   ItemOp::WRITE|
		   ItemOp::DISCOVER|
		   ItemOp::EXECUTE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_O_5_FIRMWARE_UPDATE */
#endif // WPP_FIRMWARE_UPDATE_INFO_H
