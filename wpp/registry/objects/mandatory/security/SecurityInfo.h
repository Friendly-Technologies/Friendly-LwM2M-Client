#ifndef WPP_SECURITY_INFO_H_
#define WPP_SECURITY_INFO_H_

#include "ObjectInfo.h"

namespace wpp {

static const ObjectInfo SECURITY_OBJ_INFO = {
        /* Name */
        "LWM2M Security",
        /* Object ID */
        OBJ_ID::SECURITY,
        /* URN */
        "urn:oma:lwm2m:oma:0:1.1",
        /* Object version */
        {1,1},
        /* Lwm2m version */
        {1,1},
        /* Is single */
        IS_SINGLE::MULTIPLE,
        /* Is Mandatory */
        IS_MANDATORY::MANDATORY,
        /* Object supported operations */
        Operation(Operation::READ|
                    Operation::WRITE|
                    Operation::DISCOVER|
                    Operation::EXECUTE|
                    Operation::CREATE|
                    Operation::DELETE),
};

}

#endif // WPP_SECURITY_INFO_H_