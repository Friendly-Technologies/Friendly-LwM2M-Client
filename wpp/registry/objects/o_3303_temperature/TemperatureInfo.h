/*
 * Generated on: 2024-06-27 12:56:50
 * Created by: Sinai RnD
 */

#ifndef WPP_TEMPERATURE_INFO_H
#define WPP_TEMPERATURE_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3303_TEMPERATURE

namespace wpp {

inline const ObjectInfo TEMPERATURE_OBJ_INFO = {
	/* Name */
	"Temperature",
	/* Object ID */
	OBJ_ID::TEMPERATURE,
	/* URN */
	"urn:oma:lwm2m:ext:3303",
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

#endif /* OBJ_O_3303_TEMPERATURE */
#endif // WPP_TEMPERATURE_INFO_H
