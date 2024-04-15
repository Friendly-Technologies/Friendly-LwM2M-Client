#ifndef WPP_ITEM_OPERATION_H
#define WPP_ITEM_OPERATION_H

#include <vector>

#include "WppTypes.h"

namespace wpp {

/**
 * @brief The ItemOp struct represents the operations that can be performed on a instance/resource.
 * 
 * This struct defines a set of operations that can be performed on a instance/resource. Each operation is represented
 * by a flag value, which can be combined using bitwise OR to represent multiple operations.
 * 
 * The ItemOp struct provides methods to check if a specific operation is supported, check compatibility
 * with another ItemOp object, and retrieve the flags representing the operations.
 * 
 * The ItemOp struct also provides methods to check if an operation is of a specific type, such as read, write,
 * execute, discover, delete or create.
 * 
 * Additionally, the ItemOp struct provides a method to convert the flags into a vector of operation types.
 */
struct ItemOp {
public:
	/**
	 * @brief Enum representing the different types of operations.
	 */
	enum TYPE: uint8_t {
		NONE = 0,                   /**< No operation */
	    READ = 1,                   /**< Read operation */
	    WRITE = 2,              	/**< Write update/replace operation */
	    EXECUTE = 4,                /**< Execute operation */
	    DISCOVER = 8,               /**< Discover operation */
		CREATE = 16,                 /**< Create operation */
	    DELETE = 32,                 /**< Delete operation */
	};

public:
	/**
	 * @brief Constructs a ItemOp object with the specified flags.
	 * 
	 * @param flags The flags representing the operations.
	 */
	ItemOp(uint8_t flags = TYPE::NONE): _flags(flags) {}

	/**
	 * @brief Checks if a specific operation is supported.
	 * 
	 * @param type The operation type to check.
	 * @return true if the operation is supported, false otherwise.
	 */
	inline bool isSupported(TYPE type) const { return _flags & type; };

	/**
	 * @brief Checks if the ItemOp object is compatible with another ItemOp object.
	 * 
	 * Two ItemOp objects are compatible if their flags have the same operations.
	 * 
	 * @param operation The ItemOp object to check compatibility with.
	 * @return true if the objects are compatible, false otherwise.
	 */
	inline bool isCompatible(const ItemOp& operation) const { return (_flags & operation._flags) == _flags; };

	/**
	 * @brief Checks if the ItemOp object represents a read operation.
	 * 
	 * @return true if the object represents a read operation, false otherwise.
	 */
	inline bool isRead() const { return _flags & READ; }

	/**
	 * @brief Checks if the ItemOp object represents a write operation.
	 * 
	 * @return true if the object represents a write operation, false otherwise.
	 */
	inline bool isWrite() const { return _flags & WRITE; }

	/**
	 * @brief Checks if the ItemOp object represents an execute operation.
	 * 
	 * @return true if the object represents an execute operation, false otherwise.
	 */
	inline bool isExecute() const { return _flags & EXECUTE; }

	/**
	 * @brief Checks if the ItemOp object represents a discover operation.
	 * 
	 * @return true if the object represents a discover operation, false otherwise.
	 */
	inline bool isDiscover() const { return _flags & DISCOVER; }

	/**
	 * @brief Checks if the operation is of type CREATE.
	 * 
	 * @return true if the operation is of type CREATE, false otherwise.
	 */
	inline bool isCreate() const { return _flags & CREATE; }

	/**
	 * @brief Checks if the operation is of type DELETE.
	 * 
	 * @return true if the operation is of type DELETE, false otherwise.
	 */
	inline bool isDelete() const { return _flags & DELETE; }

	/**
	 * @brief Retrieves the flags representing the operations.
	 * 
	 * @return The flags representing the operations.
	 */
	inline uint8_t getFlags() const { return _flags; }

	/**
	 * @brief Converts the flags into a vector of operation types.
	 * 
	 * @return A vector containing the operation types represented by the flags.
	 */
	inline std::vector<TYPE> asVector() const {
		uint32_t flags = _flags;
		std::vector<TYPE> operations;
		for (size_t i = 1; i != 0x100; i <<= 1) {
			if (flags & i) operations.push_back((TYPE)i);
		}
		return operations;
	}

private:
	uint8_t _flags;
};

} // namespace wpp

#endif //WPP_ITEM_OPERATION_H
