/*
 * Generated on: 2023-11-02 16:57:15
 * Created by: SinaiR&D
 */

#ifndef WPP_CONNECTIVITY_MONITORING_INFO_H
#define WPP_CONNECTIVITY_MONITORING_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_4_CONNECTIVITY_MONITORING

namespace wpp {

inline const ObjectInfo CONNECTIVITY_MONITORING_OBJ_INFO = {
	/* Name */
	"Connectivity Monitoring",
	/* Object ID */
	OBJ_ID::CONNECTIVITY_MONITORING,
	/* URN */
	"urn:oma:lwm2m:oma:4:1.3",
	/* Object version */
	{1,3},
	/* Lwm2m version */
	{1,1},
	/* Is single */
	IS_SINGLE::SINGLE,
	/* Is Mandatory */
	IS_MANDATORY::OPTIONAL,
	/* Object supported operations */
	/* --------------- Info block 0 start --------------- */
	InstOp(),
	ResOp(	ResOp::READ|
			ResOp::WRITE|
			ResOp::DISCOVER|
			ResOp::EXECUTE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_O_4_CONNECTIVITY_MONITORING */
#endif // WPP_CONNECTIVITY_MONITORING_INFO_H
