#ifndef TYPES_T
#define TYPES_T

#include <vector>
#include <string>
#include <functional>
#include <variant>

//TODO: #include "liblwm2m.h"
#include <dep.h>

namespace wpp {

#define SINGLE_INSTANCE_ID 0

using ID_T = uint16_t;
#define ID_T_MAX_VAL (LWM2M_MAX_ID)

using BOOL_T = bool;
using INT_T = int64_t;
using UINT_T = uint64_t;
using FLOAT_T = double;
using OPAQUE_T = std::vector<uint8_t>;
struct OBJ_LINK_T {
	ID_T objectId;
    ID_T objectInstanceId;
};
using STRING_T = std::string;
using CORE_LINK_T = std::string;
/*
 * Keep in mind that while std::function itself is always copy able,
 * it might hold a callable object (like a lambda) that captures
 * variables which may not be copy able. If you try to copy a
 * std::function that holds a non-copyable callable, it will compile,
 * but will throw a std::bad_function_call exception at runtime if
 * you try to call the copied std::function.
 */
using EXECUTE_T = std::function<void(ID_T, const OPAQUE_T&)>;

/*
 * Data validation function types
 */
using VERIFY_INT_T 	  	 = std::function<bool(const INT_T&)>;
using VERIFY_UINT_T 	 = std::function<bool(const UINT_T&)>;
using VERIFY_FLOAT_T	 = std::function<bool(const FLOAT_T&)>;
using VERIFY_OPAQUE_T    = std::function<bool(const OPAQUE_T&)>;
using VERIFY_BOOL_T 	 = std::function<bool(const BOOL_T&)>;
using VERIFY_OBJ_LINK_T  = std::function<bool(const OBJ_LINK_T&)>;
using VERIFY_STRING_T    = std::function<bool(const STRING_T&)>;
using VERIFY_EXECUTE_T   = std::function<bool(const EXECUTE_T)>;
using VERIFY_CORE_LINK_T = std::function<bool(const CORE_LINK_T)>;

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

struct InstanceID {
	ID_T objectId = ID_T_MAX_VAL;
    ID_T instanceId = ID_T_MAX_VAL; // TODO: use option<ID_T>
};

struct ResourceID{
    ID_T resourceId = ID_T_MAX_VAL; // TODO: use option<ID_T>
    ID_T resourceInstanceId = ID_T_MAX_VAL; // TODO: use option<ID_T>
};

struct DataID{
    InstanceID instance;
    ResourceID resource;
};

} // namespace wpp

#endif // TYPES_T
