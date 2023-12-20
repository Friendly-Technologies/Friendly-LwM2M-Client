#ifndef WPP_RES_OPERATION_H
#define WPP_RES_OPERATION_H

#include <vector>

#include "types.h"

namespace wpp {

/**
 * @brief The ResOp struct represents the operations that can be performed on a resource.
 * 
 * This struct defines a set of operations that can be performed on a resource. Each operation is represented
 * by a flag value, which can be combined using bitwise OR to represent multiple operations.
 * 
 * The ResOp struct provides methods to check if a specific operation is supported, check compatibility
 * with another ResOp object, and retrieve the flags representing the operations.
 * 
 * The ResOp struct also provides methods to check if an operation is of a specific type, such as read, write,
 * execute, discover, delete, block write, or block execute.
 * 
 * Additionally, the ResOp struct provides a method to convert the flags into a vector of operation types.
 */
struct ResOp {
public:
	/**
	 * @brief Enum representing the different types of operations.
	 */
	enum TYPE: uint16_t {
		NONE = 0,                   /**< No operation */
	    READ = 1,                   /**< Read operation */
	    WRITE_UPD = 2,              /**< Write update operation */
		WRITE_REPLACE_RES = 4,      /**< Write replace resource operation */
		WRITE_REPLACE_INST = 8,     /**< Write replace instance operation */
		WRITE = WRITE_UPD | WRITE_REPLACE_RES | WRITE_REPLACE_INST, /**< Write operation */
	    EXECUTE = 16,               /**< Execute operation */
	    DISCOVER = 32,              /**< Discover operation */
		DELETE = 64,                /**< Delete operation */
	    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
		BLOCK_WRITE = 128,          /**< Block write operation */
	    BLOCK_EXECUTE = 256,        /**< Block execute operation */
		#endif
	};

public:
	/**
	 * @brief Constructs a ResOp object with the specified flags.
	 * 
	 * @param flags The flags representing the operations.
	 */
	ResOp(uint16_t flags = TYPE::NONE): _flags(flags) {}

	/**
	 * @brief Checks if a specific operation is supported.
	 * 
	 * @param type The operation type to check.
	 * @return true if the operation is supported, false otherwise.
	 */
	inline bool isSupported(TYPE type) const { return _flags & type; };

	/**
	 * @brief Checks if the ResOp object is compatible with another ResOp object.
	 * 
	 * Two ResOp objects are compatible if their flags have the same operations.
	 * 
	 * @param operation The ResOp object to check compatibility with.
	 * @return true if the objects are compatible, false otherwise.
	 */
	inline bool isCompatible(const ResOp& operation) const { return (_flags & operation._flags) == _flags; };

	/**
	 * @brief Checks if the ResOp object represents a read operation.
	 * 
	 * @return true if the object represents a read operation, false otherwise.
	 */
	inline bool isRead() const { return _flags & READ; }

	/**
	 * @brief Checks if the ResOp object represents a write operation.
	 * 
	 * @return true if the object represents a write operation, false otherwise.
	 */
	inline bool isWrite() const { return _flags & WRITE; }

	/**
	 * @brief Checks if the ResOp object represents an execute operation.
	 * 
	 * @return true if the object represents an execute operation, false otherwise.
	 */
	inline bool isExecute() const { return _flags & EXECUTE; }

	/**
	 * @brief Checks if the ResOp object represents a discover operation.
	 * 
	 * @return true if the object represents a discover operation, false otherwise.
	 */
	inline bool isDiscover() const { return _flags & DISCOVER; }

	/**
	 * @brief Checks if the ResOp object represents a delete operation.
	 * 
	 * @return true if the object represents a delete operation, false otherwise.
	 */
	inline bool isDelete() const { return _flags & DELETE; }

	#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	/**
	 * @brief Checks if the ResOp object represents a block write operation.
	 * 
	 * @return true if the object represents a block write operation, false otherwise.
	 */
	inline bool isBlockWrite() const { return _flags & BLOCK_WRITE; }

	/**
	 * @brief Checks if the ResOp object represents a block execute operation.
	 * 
	 * @return true if the object represents a block execute operation, false otherwise.
	 */
	inline bool isBlockExecute() const { return _flags & BLOCK_EXECUTE; }
	#endif

	/**
	 * @brief Retrieves the flags representing the operations.
	 * 
	 * @return The flags representing the operations.
	 */
	inline uint16_t getFlags() const { return _flags; }

	/**
	 * @brief Converts the flags into a vector of operation types.
	 * 
	 * @return A vector containing the operation types represented by the flags.
	 */
	inline std::vector<TYPE> asVector() const {
		uint32_t flags = _flags;
		std::vector<TYPE> operations;
		for (size_t i = 1; i != 0x1000; i <<= 1) {
			if (flags & i) operations.push_back((TYPE)i);
		}
		return operations;
	}

private:
	uint16_t _flags;
};

} // namespace wpp

#endif //WPP_RES_OPERATION_H
