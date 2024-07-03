/*
 * Generated on: 2024-06-27 12:57:44
 * Created by: Sinai RnD
 */

#ifndef WPP_ACCELEROMETER_INFO_H
#define WPP_ACCELEROMETER_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3313_ACCELEROMETER

namespace wpp {

inline const ObjectInfo ACCELEROMETER_OBJ_INFO = {
	/* Name */
	"Accelerometer",
	/* Object ID */
	OBJ_ID::ACCELEROMETER,
	/* URN */
	"urn:oma:lwm2m:ext:3313",
	/* Object version */
	{1,0},
	/* Lwm2m version */
	{1,0},
	/* Is single */
	IS_SINGLE::MULTIPLE,
	/* Is Mandatory */
	IS_MANDATORY::OPTIONAL,
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

#endif /* OBJ_O_3313_ACCELEROMETER */
#endif // WPP_ACCELEROMETER_INFO_H
