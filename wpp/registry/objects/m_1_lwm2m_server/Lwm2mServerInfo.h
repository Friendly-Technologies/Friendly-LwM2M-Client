/*
 * Generated on: 2023-11-02 16:41:01
 * Created by: Sinai RnD
 */

#ifndef WPP_LWM2M_SERVER_INFO_H
#define WPP_LWM2M_SERVER_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_M_1_LWM2M_SERVER

namespace wpp {

inline const ObjectInfo LWM2M_SERVER_OBJ_INFO = {
	/* Name */
	"LwM2M Server",
	/* Object ID */
	OBJ_ID::LWM2M_SERVER,
	/* URN */
	"urn:oma:lwm2m:oma:1:1.1",
	/* Object version */
	{1,1},
	/* Lwm2m version */
	{1,1},
	/* Is single */
	IS_SINGLE::MULTIPLE,
	/* Is Mandatory */
	IS_MANDATORY::MANDATORY,
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

#endif /* OBJ_M_1_LWM2M_SERVER */
#endif // WPP_LWM2M_SERVER_INFO_H
