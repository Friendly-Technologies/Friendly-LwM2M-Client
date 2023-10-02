#ifndef WPP_DEVICE_INFO_H_
#define WPP_DEVICE_INFO_H_

#include "ObjectInfo.h"

namespace wpp {

static const ObjectInfo DEVICE_OBJ_INFO = {
            /* Name */
            "Device",
            /* Object ID */
            OBJ_ID::DEVICE,
            /* URN */
            "urn:oma:lwm2m:oma:3:1.1",
            /* Object version */
            {1,1},
            /* Lwm2m version */
            {1,1},
            /* Is single */
            IS_SINGLE::SINGLE,
            /* Is Mandatory */
            IS_MANDATORY::MANDATORY,
            /* Object supported operations */
            InstOp(InstOp::CREATE),
            ResOp(ResOp::READ|
                         ResOp::WRITE|
                         ResOp::EXECUTE|
                         ResOp::DISCOVER),
    };

}

#endif // WPP_DEVICE_INFO_H_