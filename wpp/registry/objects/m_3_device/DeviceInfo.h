/*
 * Generated on: 2023-11-02 16:38:01
 * Created by: Sinai RnD
 */

#ifndef WPP_DEVICE_INFO_H
#define WPP_DEVICE_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_M_3_DEVICE

namespace wpp {

inline const ObjectInfo DEVICE_OBJ_INFO = {
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
	ItemOp(ItemOp::READ|
		   ItemOp::WRITE|
		   ItemOp::DISCOVER|
		   ItemOp::EXECUTE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_M_3_DEVICE */
#endif // WPP_DEVICE_INFO_H
