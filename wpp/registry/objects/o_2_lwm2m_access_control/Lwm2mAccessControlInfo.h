/*
 * Generated on: 2023-11-03 15:05:47
 * Created by: Sinai RnD
 */

#ifndef WPP_LWM2M_ACCESS_CONTROL_INFO_H
#define WPP_LWM2M_ACCESS_CONTROL_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL

namespace wpp {

inline const ObjectInfo LWM2M_ACCESS_CONTROL_OBJ_INFO = {
	/* Name */
	"LwM2M Access Control",
	/* Object ID */
	OBJ_ID::LWM2M_ACCESS_CONTROL,
	/* URN */
	"urn:oma:lwm2m:oma:2:1.1",
	/* Object version */
	{1,1},
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

#endif /* OBJ_O_2_LWM2M_ACCESS_CONTROL */
#endif // WPP_LWM2M_ACCESS_CONTROL_INFO_H
