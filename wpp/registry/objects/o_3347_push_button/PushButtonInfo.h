/*
 * Generated on: 2024-06-23 17:09:01
 * Created by: Sinai RnD
 */

#ifndef WPP_PUSH_BUTTON_INFO_H
#define WPP_PUSH_BUTTON_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3347_PUSH_BUTTON

namespace wpp {

inline const ObjectInfo PUSH_BUTTON_OBJ_INFO = {
	/* Name */
	"Push button",
	/* Object ID */
	OBJ_ID::PUSH_BUTTON,
	/* URN */
	"urn:oma:lwm2m:ext:3347",
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

#endif /* OBJ_O_3347_PUSH_BUTTON */
#endif // WPP_PUSH_BUTTON_INFO_H
