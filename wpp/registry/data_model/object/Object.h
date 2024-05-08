/*
 * Object.h
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_OBJECT_H_
#define WPP_OBJECT_H_

#include <vector>
#include <variant>

#include "ObjSubject.h"
#include "Instance.h"
#include "WppTypes.h"
#include "WppLogs.h"

namespace wpp {

class WppRegistry;
class WppClient;

/**
 * @class Object
 * @brief The Object class implements manipulation with Instance interface class and its inheritors.
 * 
 * The main target of this class is to encapsulate operations like instance create and delete, binding
 * instance callbacks to the core interface, to avoid multiple definitions of this mechanism in instance
 * implementation classes.
 */
class Object : public ObjSubject {
private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

public:
	/**
	 * Default constructor.
	 */
	Object(lwm2m_context_t &context);

	/**
	 * @brief Constructs an Object instance.
	 * 
	 * @param context The lwm2m_context_t object.
	 * @param info The ObjectInfo object.
	 */
	Object(lwm2m_context_t &context, const ObjectInfo &info);

	/**
	 * @brief Destroys the Object instance.
	 */
	virtual ~Object();

	/* ------------- Object management ------------- */

	/**
	 * @brief Gets the object ID.
	 * 
	 * @return The object ID.
	 */
	OBJ_ID getObjectID() const;

	/**
	 * @brief Gets the object information.
	 * 
	 * @return The ObjectInfo object.
	 */
	const ObjectInfo& getObjectInfo() const;

	/**
	 * @brief Gets the lwm2m_object_t object.
	 * 
	 * @return The lwm2m_object_t object.
	 */
	lwm2m_object_t& getLwm2mObject();

	/**
	 * @brief Return context that can be used by derived class.
	 */
	lwm2m_context_t& getContext();

	/**
 	 * @brief Helpfull methods to get client instances. 
	 */
	WppClient& getClient();

	/**
	 * @brief Helpfull methods to get registry instances. 
	 */
	WppRegistry& getRegistry();

	/* ------------- Object instance management ------------- */

	/**
	 * @brief Creates an instance of the object.
	 * 
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object.
	 */
	virtual Instance* createInstance(ID_T instanceID = ID_T_MAX_VAL) = 0;

	/**
	 * @brief Removes an instance of the object.
	 * 
	 * @param instanceID The instance ID.
	 * @return True if the instance was successfully removed, false otherwise.
	 */
	bool remove(ID_T instanceID);

	/**
	 * @brief Clears the object.
	 */
	void clear();

	/**
	 * @brief Gets an instance of the object.
	 * 
	 * @param instanceID The instance ID. If not provided, the first available instance is returned.
	 * @return A pointer to the Instance object or NULL.
	 */
	Instance* instance(ID_T instanceID = ID_T_MAX_VAL);

	/**
	 * @brief Gets all instances of the object.
	 * 
	 * @return A vector of pointers to the Instance objects.
	 */
	const std::vector<Instance*>& instances();

	/**
	 * @brief Gets the number of instances of the object.
	 * 
	 * @return The number of instances.
	 */
	size_t instanceCnt();

	/**
	 * @brief Checks if an instance exists.
	 * 
	 * @param instanceID The instance ID.
	 * @return True if the instance exists, false otherwise.
	 */
	bool isExist(ID_T instanceID);

protected:
	/**
	 * @brief Gets the iterator to the instance.
	 * 
	 * @param instanceID The instance ID.
	 * @return The iterator to the instance.
	 */
	std::vector<Instance*>::iterator getInstIter(ID_T instanceID);

	/**
	 * @brief Gets the first available instance ID.
	 * 
	 * @return The first available instance ID.
	 */
	ID_T getFirstAvailableInstanceID();

	/* ------------- Lwm2m core callback ------------- */

	/**
	 * @brief The read callback function for the Lwm2m core.
	 * 
	 * @param contextP The lwm2m_context_t object.
	 * @param server Contains valid pointer when request retrieved from server or NULL if request initiated by core.
	 * @param instanceId The instance ID.
	 * @param numDataP The number of data.
	 * @param dataArrayP The data array.
	 * @param objectP The lwm2m_object_t object.
	 * @return The result of the read operation.
	 */
	static uint8_t serverRead_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);

	/**
	 * @brief The write callback function for the Lwm2m core.
	 * 
	 * @param contextP The lwm2m_context_t object.
	 * @param server Contains valid pointer when request retrieved from server or NULL if request initiated by core.
	 * @param instanceId The instance ID.
	 * @param numData The number of data.
	 * @param dataArray The data array.
	 * @param objectP The lwm2m_object_t object.
	 * @param writeType The write type.
	 * @return The result of the write operation.
	 */
	static uint8_t serverWrite_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP, lwm2m_write_type_t writeType);

	/**
	 * @brief The execute callback function for the Lwm2m core.
	 * 
	 * @param contextP The lwm2m_context_t object.
	 * @param server Contains valid pointer when request retrieved from server or NULL if request initiated by core.
	 * @param instanceId The instance ID.
	 * @param resId The resource ID.
	 * @param buffer The buffer.
	 * @param length The length.
	 * @param objectP The lwm2m_object_t object.
	 * @return The result of the execute operation.
	 */
	static uint8_t serverExecute_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, ID_T resId, uint8_t * buffer, int length, lwm2m_object_t * objectP);

	/**
	 * @brief The discover callback function for the Lwm2m core.
	 * 
	 * @param contextP The lwm2m_context_t object.
	 * @param server Contains valid pointer when request retrieved from server or NULL if request initiated by core.
	 * @param instanceId The instance ID.
	 * @param numDataP The number of data.
	 * @param dataArrayP The data array.
	 * @param objectP The lwm2m_object_t object.
	 * @return The result of the discover operation.
	 */
	static uint8_t serverDiscover_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);

	/**
	 * @brief The create callback function for the Lwm2m core.
	 * 
	 * @param contextP The lwm2m_context_t object.
	 * @param server Contains valid pointer when request retrieved from server or NULL if request initiated by core.
	 * @param instanceId The instance ID.
	 * @param numData The number of data.
	 * @param dataArray The data array.
	 * @param objectP The lwm2m_object_t object.
	 * @return The result of the create operation.
	 */
	static uint8_t serverCreate_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

	/**
	 * @brief The delete callback function for the Lwm2m core.
	 * 
	 * @param contextP The lwm2m_context_t object.
	 * @param server Contains valid pointer when request retrieved from server or NULL if request initiated by core.
	 * @param instanceId The instance ID.
	 * @param objectP The lwm2m_object_t object.
	 * @return The result of the delete operation.
	 */
	static uint8_t serverDelete_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, lwm2m_object_t * objectP);

protected:
	std::vector<Instance*> _instances;
	lwm2m_context_t &_context;
	lwm2m_object_t _lwm2m_object;
	ObjectInfo _objInfo;
};

} // namespace wpp

#endif /* WPP_OBJECT_H_ */
