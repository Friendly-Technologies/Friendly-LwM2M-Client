#ifndef DEVICEINFO_H_
#define DEVICEINFO_H_

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
            Operation(Operation::READ|
                        Operation::WRITE|
                        Operation::DISCOVER|
                        Operation::EXECUTE|
                        Operation::CREATE),
    };

}

#endif // DEVICEINFO_H_