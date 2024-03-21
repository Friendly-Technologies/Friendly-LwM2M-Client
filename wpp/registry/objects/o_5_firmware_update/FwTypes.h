#ifndef WPP_FW_UPD_TYPES_H
#define WPP_FW_UPD_TYPES_H

#include "WppTypes.h"
#include "FirmwareUpdateConfig.h"

namespace wpp {

enum FwUpdRes: uint8_t {
    R_INITIAL = 0,
    R_FW_UPD_SUCCESS = 1,
    R_NOT_ENOUGH_FLASH = 2,
    R_OUT_OF_RAM = 3,
    R_CONN_LOST = 4,
    R_INTEGRITY_CHECK_FAIL = 5,
    R_UNSUPPORTED_PKG_TYPE = 6,
    R_INVALID_URI = 7,
    R_FW_UPD_FAIL = 8,
    R_UNSUPPORTED_PROTOCOL = 9,
    UPD_RES_MAX
};

#if RES_5_8
enum FwUpdProtocol: uint8_t {
    COAP = 0,
    COAPS = 1,
    HTTP = 2,
    HTTPS = 3,
    COAP_TCP = 4,
    COAP_TLS = 5,
    FW_UPD_PROTOCOL_MAX
};
#endif

enum FwUpdState: uint8_t {
    S_IDLE = 0,
    S_DOWNLOADING = 1,
    S_DOWNLOADED = 2,
    S_UPDATING = 3,
    STATE_MAX
};

enum FwUpdDelivery: uint8_t {
    PULL = 0,
    PUSH = 1,
    BOTH = 2,
    FW_UPD_DELIVERY_MAX
};

} // namespace wpp

#endif // WPP_FW_UPD_TYPES_H
