#ifndef WPP_RES_BLOCK_OPERATION_H
#define WPP_RES_BLOCK_OPERATION_H

#include <vector>

#include "types.h"

namespace wpp {

struct ResBlockOp {
public:
	enum TYPE: uint8_t {
        NONE = 0,
	    BLOCK_WRITE = 1,
	    BLOCK_EXECUTE = 2,
	};

public:
	ResBlockOp(uint8_t flags = TYPE::NONE): _flags(flags) {}
    inline bool isSupported(TYPE type) const { return _flags & type; };
    inline bool isCompatible(const ResBlockOp& operation) const { return (_flags & operation._flags) == _flags; };
    inline bool isBlockWrite() const { return _flags & BLOCK_WRITE; }
    inline bool isBlockExecute() const { return _flags & BLOCK_EXECUTE; }
    inline uint8_t getFlags() const { return _flags; }
    inline std::vector<TYPE> asVector() const {
    	uint32_t flags = _flags;
    	std::vector<TYPE> operations;
    	for (size_t i = 1; i != 0x10; i <<= 1) {
    		if (flags & i) operations.push_back((TYPE)i);
    	}
    	return operations;
    }

private:
    uint8_t _flags;
};

} // namespace wpp

#endif //WPP_RES_BLOCK_OPERATION_H
