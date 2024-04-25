#ifndef WPP_TYPES_T
#define WPP_TYPES_T

#include <vector>
#include <string>
#include <functional>
#include <variant>

#include "liblwm2m.h"

namespace wpp {

#define SINGLE_INSTANCE_ID 0

using ID_T = uint16_t;
#define ID_T_MAX_VAL (LWM2M_MAX_ID)

using EVENT_ID_T = uint8_t;

/**
 * @brief Wpp data types ID
 */
enum class TYPE_ID: uint8_t {
	BOOL,           // bool
	INT,            // int64_t
	UINT,           // uint64_t
	FLOAT,          // double
	OBJ_LINK,       // {object ID, instance ID}
	TIME,         	// Derived from INT
	OPAQUE,    		// vector<uint8_t>
	STRING,    		// string
	CORE_LINK, 		// Derived from STRING
	EXECUTE,		// Type of executable resources
	UNDEFINED     	// Undefined type
};

/**
 * @brief Wpp data types bindings
 */
using BOOL_T = bool;
using INT_T = int64_t;
using UINT_T = uint64_t;
using FLOAT_T = double;
using TIME_T = INT_T;
using STRING_T = std::string;
/**
 * @brief Opaque - represent buffer or string as lwm2m_data_t.value.asBuffer
 */
using OPAQUE_T = std::vector<uint8_t>;
/**
 * @brief ObjLink - (object ID):(instance ID), example: 1:3. 
 * Represent as two integers in lwm2m_data_t.value.asObjLink.
 */
struct OBJ_LINK_T {
	ID_T objId = ID_T_MAX_VAL;
    ID_T objInstId = ID_T_MAX_VAL;
};
/**
 * @brief CoreLink -  </3/0> or </1/0/>;ssid=101 or </5>,</4>,</55>;ver=1.9,</55/0>.
 * Represent as string in lwm2m_data_t.value.asBuffer
 */
using CORE_LINK_T = std::string;
/**
 * @brief Keep in mind that while std::function itself is always copy able,
 * it might hold a callable object (like a lambda) that captures
 * variables which may not be copy able. If you try to copy a
 * std::function that holds a non-copyable callable, it will compile,
 * but will throw a std::bad_function_call exception at runtime if
 * you try to call the copied std::function.
 */
class Instance;
using EXECUTE_T = std::function<bool(Instance&, ID_T, const OPAQUE_T&)>;

/**
 * @brief Determining type ID by real type
 */
template<typename T>
TYPE_ID dataTypeToID() {
	TYPE_ID typeID = TYPE_ID::UNDEFINED;
	if constexpr (std::is_same<T, BOOL_T>::value) typeID = TYPE_ID::BOOL;
	else if constexpr (std::is_same<T, INT_T>::value) typeID = TYPE_ID::INT;
	else if constexpr (std::is_same<T, UINT_T>::value) typeID = TYPE_ID::UINT;
	else if constexpr (std::is_same<T, FLOAT_T>::value) typeID = TYPE_ID::FLOAT;
	else if constexpr (std::is_same<T, OPAQUE_T>::value) typeID = TYPE_ID::OPAQUE;
	else if constexpr (std::is_same<T, OBJ_LINK_T>::value) typeID = TYPE_ID::OBJ_LINK;
	else if constexpr (std::is_same<T, STRING_T>::value) typeID = TYPE_ID::STRING;
	else if constexpr (std::is_same<T, EXECUTE_T>::value) typeID = TYPE_ID::EXECUTE;
	return typeID;
}

/**
 * @brief Data validation function types
 */
using VERIFY_INT_T 	  	 = std::function<bool(const INT_T&)>;
using VERIFY_UINT_T 	 = std::function<bool(const UINT_T&)>;
using VERIFY_FLOAT_T	 = std::function<bool(const FLOAT_T&)>;
using VERIFY_OPAQUE_T    = std::function<bool(const OPAQUE_T&)>;
using VERIFY_BOOL_T 	 = std::function<bool(const BOOL_T&)>;
using VERIFY_OBJ_LINK_T  = std::function<bool(const OBJ_LINK_T&)>;
using VERIFY_STRING_T    = std::function<bool(const STRING_T&)>;
using VERIFY_EXECUTE_T   = std::function<bool(const EXECUTE_T &)>;
using VERIFY_CORE_LINK_T = std::function<bool(const CORE_LINK_T &)>;

enum class PRIORITY: uint8_t {
	HIGH,
	MEDIUM,
	LOW,
	DEFAULT = LOW
};

enum class IS_SINGLE: uint8_t {
	SINGLE,
	MULTIPLE
};

enum class IS_MANDATORY: uint8_t {
	MANDATORY,
	OPTIONAL
};


struct Version {
    uint8_t major;
    uint8_t minor;
};

struct ResLink {
    ID_T resId = ID_T_MAX_VAL;
    ID_T resInstId = ID_T_MAX_VAL;
};

struct DataLink {
    OBJ_LINK_T instance;
    ResLink resource;
};

} // namespace wpp

#endif // WPP_TYPES_T
