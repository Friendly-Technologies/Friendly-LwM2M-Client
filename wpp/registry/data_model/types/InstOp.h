#ifndef WPP_INST_OPERATION_H
#define WPP_INST_OPERATION_H

#include <vector>

#include "WppTypes.h"

/**
 * @brief The InstOp struct represents an instance operation.
 * 
 * This struct defines the different types of operations that can be performed on an instnce.
 * The operations include CREATE, DELETE, and BLOCK1_CREATE.
 * 
 * The InstOp struct provides methods to check the supported types of operations,
 * check compatibility with another InstOp object, and retrieve the flags associated with the operation.
 * It also provides a method to convert the flags into a vector of operation types.
 * 
 * @note The InstOp struct is part of the wpp namespace.
 */
namespace wpp {

struct InstOp {
public:
	/**
	 * @brief The TYPE enum represents the different types of instnce operations.
	 */
	enum TYPE: uint8_t {
		NONE = 0,           /**< No operation */
		CREATE = 1,         /**< Create operation */
	    DELETE = 2,         /**< Delete operation */
		#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	    BLOCK1_CREATE = 4,  /**< Block1 create operation */
		#endif
	};

public:
	/**
	 * @brief Constructs an InstOp object with the specified flags.
	 * 
	 * @param flags The flags associated with the operation.
	 */
	InstOp(uint8_t flags = TYPE::NONE): _flags(flags) {}

	/**
	 * @brief Checks if the specified operation type is supported.
	 * 
	 * @param type The operation type to check.
	 * @return true if the operation type is supported, false otherwise.
	 */
	inline bool isSupported(TYPE type) const { return _flags & type; };

	/**
	 * @brief Checks if the current operation is compatible with another InstOp object.
	 * 
	 * @param operation The InstOp object to check compatibility with.
	 * @return true if the operations are compatible, false otherwise.
	 */
	inline bool isCompatible(const InstOp& operation) const { return (_flags & operation._flags) == _flags; };

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

	#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	/**
	 * @brief Checks if the operation is of type BLOCK1_CREATE.
	 * 
	 * @return true if the operation is of type BLOCK1_CREATE, false otherwise.
	 */
	inline bool isBlockCreate() const { return _flags & BLOCK1_CREATE; }
	#endif

	/**
	 * @brief Retrieves the flags associated with the operation.
	 * 
	 * @return The flags associated with the operation.
	 */
	inline uint8_t getFlags() const { return _flags; }

	/**
	 * @brief Converts the flags into a vector of operation types.
	 * 
	 * @return A vector of operation types.
	 */
	inline std::vector<TYPE> asVector() const {
		uint16_t flags = _flags;
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

#endif //WPP_INST_OPERATION_H
