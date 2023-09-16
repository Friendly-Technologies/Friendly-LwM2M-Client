#ifndef WPPCONFIG_H_
#define WPPCONFIG_H_

/* ---------- Loging start ---------- */

// Enable logs for WakaamaPlus
#define ENABLE_LOGS

/* ---------- Loging end ---------- */

/* ---------- Mandatory objects start ---------- */
/* ---------- Mandatory objects end ---------- */

/* ---------- Optional objects start ---------- */

// Include ACL object in the build
#define ACL_OBJ
#ifdef ACL_OBJ
#endif // ACL_OBJ

// Include Connection monitoring object in the build
#define CONN_MONITORING_OBJ
#ifdef CONN_MONITORING_OBJ
#endif // CONN_MONITORING_OBJ

// Include Firmware update in the build
#define FIRMWARE_UPD_OBJ
#ifdef FIRMWARE_UPD_OBJ
#endif // FIRMWARE_UPD_OBJ

/* ---------- Optional objects end ---------- */

#endif //WPPCONFIG_H_