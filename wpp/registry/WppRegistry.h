/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPPREGISTRY_H_
#define WPPREGISTRY_H_

#include "Object.h"
#include "utils.h"
#include "config.h"

#include "security/Security.h"
#include "server/Server.h"
#include "device/Device.h"

namespace wpp {

// TODO: Add ability to check whether some object id is exist
class WppRegistry {
private:
	WppRegistry() {}

public:
	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	/* ------------- Registry management ------------- */
	static WppRegistry& instance() {
		static WppRegistry registry;
		return registry;
	}

	/* ------------- Objects management ------------- */
	Object<Security>& security() {
		static const ObjectInfo info = {
				"Server",					// Name
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

		if (!Object<Security>::isCreated()) Object<Security>::create(info);
		return *Object<Security>::object();
	}

	Object<Server>& server() {
		static const ObjectInfo info = {
				"Server",					// Name
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
		return *Object<Server>::object();
	}

	Object<Device>& device() {
		static const ObjectInfo info = {
				"Server",					// Name
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

		if (!Object<Device>::isCreated()) Object<Device>::create(info);
		return *Object<Device>::object();
	}
};

} // namespace wpp

#endif // WPPREGISTRY_H_
