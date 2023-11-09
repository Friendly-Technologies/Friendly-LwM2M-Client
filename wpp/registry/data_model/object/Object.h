/*
 * Object.h
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_OBJECT_H_
#define WPP_OBJECT_H_

#include <unordered_map>
#include <variant>

#include "ObjSubject.h"
#include "Instance.h"
#include "types.h"
#include "WppLogs.h"

namespace wpp {

/*
 * Object<T> is class that implements manipulation with Instance interface class and his inheritors.
 * The main target of this class is to encapsulate operations like instance create and delete, binding
 * instance callbacks to core interface, for avoid multiple definition of this mechanism in instance
 * implementation classes.
 */
class Object : public ObjSubject {
private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

public:
    Object(lwm2m_context_t &context, const ObjectInfo &info);
	virtual ~Object();

    /* ------------- Object management ------------- */
    OBJ_ID getObjectID() const;
	const ObjectInfo& getObjectInfo() const;
	lwm2m_object_t& getLwm2mObject();
    void clear();
	void restore();

    /* ------------- Object instance management ------------- */
	virtual Instance* createInstance(ID_T instanceID = ID_T_MAX_VAL) = 0;
	bool removeInstance(ID_T instanceID);
	Instance* instance(ID_T instanceID = 0);
	const std::vector<Instance*> & getInstances();
    size_t instanceCnt();
	bool isInstanceExist(ID_T instanceID);

protected:
	std::vector<Instance*>::iterator getInstIter(ID_T instanceID);
	ID_T getFirstAvailableInstanceID();

    /* ------------- Lwm2m core callback ------------- */
	static uint8_t read_clb(lwm2m_context_t * contextP, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);
	static uint8_t write_clb(lwm2m_context_t * contextP, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP, lwm2m_write_type_t writeType);
	static uint8_t execute_clb(lwm2m_context_t * contextP, ID_T instanceId, ID_T resId, uint8_t * buffer, int length, lwm2m_object_t * objectP);
	static uint8_t discover_clb(lwm2m_context_t * contextP, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);
	static uint8_t create_clb(lwm2m_context_t * contextP, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
	static uint8_t delete_clb(lwm2m_context_t * contextP, ID_T instanceId, lwm2m_object_t * objectP);
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	// TODO Implement this functionality
	static uint8_t block1Ccreate_clb(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_media_type_t format, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
	static uint8_t block1Write_clb(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_media_type_t format, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
	static uint8_t block1Execute_clb(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
#endif

protected:
	std::vector<Instance*> _instances;
	lwm2m_context_t &_context;
    lwm2m_object_t _lwm2m_object;
	ObjectInfo _objInfo;
};

} // namespace wpp

#endif /* WPP_OBJECT_H_ */
