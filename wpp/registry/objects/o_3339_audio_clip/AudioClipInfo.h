/*
 * Generated on: 2024-03-11 13:52:27
 * Created by: Sinai RnD
 */

#ifndef WPP_AUDIO_CLIP_INFO_H
#define WPP_AUDIO_CLIP_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3339_AUDIO_CLIP

namespace wpp {

inline const ObjectInfo AUDIO_CLIP_OBJ_INFO = {
	/* Name */
	"Audio Clip",
	/* Object ID */
	OBJ_ID::AUDIO_CLIP,
	/* URN */
	"urn:oma:lwm2m:ext:3339",
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

#endif /* OBJ_O_3339_AUDIO_CLIP */
#endif // WPP_AUDIO_CLIP_INFO_H
