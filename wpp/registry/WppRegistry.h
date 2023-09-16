/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPPREGISTRY_H_
#define WPPREGISTRY_H_

#include "Object.h"

#include "mandatory/security/Security.h"
#include "mandatory/server/Server.h"
#include "mandatory/device/Device.h"

#ifdef ACL_OBJ
#include "optional/acl/Acl.h"
#endif
#ifdef CONN_MONITORING_OBJ
#include "optional/conn_monitoring/ConnMonitoring.h"
#endif
#ifdef FIRMWARE_UPD_OBJ
#include "optional/firmware_upd/FirmwareUpd.h"
#endif

namespace wpp {

// TODO: Split mandatory and optional registers
// TODO: Add ability to check whether some object id is exist
// TODO: Add ability to get Lwm2mObject by ID
class WppRegistry {
public:
	/*
	 * OBJ_RESTORE_T represents callback that notifies user about the requirement of restoring the object state to default
	 *
	 * NOTE: Keep in mind that while std::function itself is always copy able,
	 * it might hold a callable object (like a lambda) that captures
	 * variables which may not be copy able. If you try to copy a
	 * std::function that holds a non-copyable callable, it will compile,
	 * but will throw a std::bad_function_call exception at runtime if
	 * you try to call the copied std::function.
	 */
	using OBJ_RESTORE_T = std::function<void(Lwm2mObject &)>;

public:
	WppRegistry(const OBJ_RESTORE_T &objRestoreFunc) : _objRestoreFunc(objRestoreFunc) {}
	~WppRegistry() {}

	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	/* ------------- Registry functionality ------------- */
	void restoreObject(Lwm2mObject &object) {
		if (_objRestoreFunc) _objRestoreFunc(object);
		else object.clear(); // TODO: Validate this behaviour
	}

	/* ------------- Mandatory objects ------------- */
	Object<Security>& security() {
		static const ObjectInfo info = {
				"LWM2M Security",			// Name
				OBJ_ID::SECURITY,			// Object ID
				"urn:oma:lwm2m:oma:0:1.1",	// URN
				{1,1},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::MULTIPLE,		// Is single
				IS_MANDATORY::MANDATORY,	// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Security>::isCreated()) Object<Security>::create(info);
		return *Object<Security>::instance();
	}

	Object<Server>& server() {
		static const ObjectInfo info = {
				"LwM2M Server",				// Name
				OBJ_ID::SERVER,				// Object ID
				"urn:oma:lwm2m:oma:1:1.1",	// URN
				{1,1},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::MULTIPLE,		// Is single
				IS_MANDATORY::MANDATORY,	// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Server>::isCreated()) Object<Server>::create(info);
		return *Object<Server>::instance();
	}

	Object<Device>& device() {
		static const ObjectInfo info = {
				"Device",					// Name
				OBJ_ID::DEVICE,				// Object ID
				"urn:oma:lwm2m:oma:3:1.1",	// URN
				{1,1},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::SINGLE,			// Is single
				IS_MANDATORY::MANDATORY,	// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Device>::isCreated()) Object<Device>::create(info);
		return *Object<Device>::instance();
	}

	/* ------------- Optional objects ------------- */
	#ifdef ACL_OBJ
	Object<Acl>& acl() {
		static const ObjectInfo info = {
				"LwM2M Access Control",		// Name
				OBJ_ID::ACL,				// Object ID
				"urn:oma:lwm2m:oma:2",		// URN
				{1,0},						// Object version
				{1,0},						// Lwm2m version
				IS_SINGLE::MULTIPLE,		// Is single
				IS_MANDATORY::OPTIONAL,		// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<Acl>::isCreated()) Object<Acl>::create(info);
		return *Object<Acl>::instance();
	}
	#endif

	#ifdef CONN_MONITORING_OBJ
	Object<ConnMonitoring>& connMonitoring() {
		static const ObjectInfo info = {
				"Connectivity Monitoring",	// Name
				OBJ_ID::CONN_MONITORING,	// Object ID
				"urn:oma:lwm2m:oma:4:1.2 ",	// URN
				{1,2},						// Object version
				{1,1},						// Lwm2m version
				IS_SINGLE::SINGLE,			// Is single
				IS_MANDATORY::OPTIONAL,		// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<ConnMonitoring>::isCreated()) Object<ConnMonitoring>::create(info);
		return *Object<ConnMonitoring>::instance();
	}
	#endif

	#ifdef FIRMWARE_UPD_OBJ
		Object<FirmwareUpd>& firmwareUpd() {
		static const ObjectInfo info = {
				"Firmware Update",			// Name
				OBJ_ID::ACFIRMWARE_UPDL,	// Object ID
				"urn:oma:lwm2m:oma:5",		// URN
				{1,0},						// Object version
				{1,0},						// Lwm2m version
				IS_SINGLE::SINGLE,			// Is single
				IS_MANDATORY::OPTIONAL,		// Is Mandatory
				Operation(Operation::READ|	// Object supported operations
						  Operation::WRITE|
						  Operation::DISCOVER|
						  Operation::EXECUTE|
						  Operation::CREATE|
						  Operation::DELETE),
		};

		if (!Object<FirmwareUpd>::isCreated()) Object<FirmwareUpd>::create(info);
		return *Object<FirmwareUpd>::instance();
	}
	#endif

private:
	OBJ_RESTORE_T _objRestoreFunc;
};

} // namespace wpp

#endif // WPPREGISTRY_H_
