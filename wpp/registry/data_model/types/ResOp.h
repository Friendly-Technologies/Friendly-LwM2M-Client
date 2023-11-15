#ifndef WPP_RES_OPERATION_H
#define WPP_RES_OPERATION_H

#include <vector>

#include "types.h"

namespace wpp {

struct ResOp {
public:
	enum TYPE: uint16_t {
        NONE = 0,
	    READ = 1,
	    WRITE_UPD = 2,
		WRITE_REPLACE_RES = 4,
		WRITE_REPLACE_INST = 8,
		WRITE = WRITE_UPD | WRITE_REPLACE_RES | WRITE_REPLACE_INST,
	    EXECUTE = 16,
	    DISCOVER = 32,
		DELETE = 64,
	    BLOCK_WRITE = 128,
	    BLOCK_EXECUTE = 256,
	};

public:
	ResOp(uint16_t flags = TYPE::NONE): _flags(flags) {}
    inline bool isSupported(TYPE type) const { return _flags & type; };
    inline bool isCompatible(const ResOp& operation) const { return (_flags & operation._flags) == _flags; };
    inline bool isRead() const { return _flags & READ; }
    inline bool isWrite() const { return _flags & WRITE; }
    inline bool isExecute() const { return _flags & EXECUTE; }
    inline bool isDiscover() const { return _flags & DISCOVER; }
	inline bool isDelete() const { return _flags & DELETE; }
    inline bool isBlockWrite() const { return _flags & BLOCK_WRITE; }
    inline bool isBlockExecute() const { return _flags & BLOCK_EXECUTE; }
    inline uint16_t getFlags() const { return _flags; }
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
