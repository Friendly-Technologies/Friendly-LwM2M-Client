#ifndef WPP_RES_OPERATION_H
#define WPP_RES_OPERATION_H

#include <vector>

#include "types.h"

namespace wpp {

struct ResOperation {
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
	    BLOCK1_WRITE = 128,
	    BLOCK1_EXECUTE = 256,
	};

public:
	ResOperation(uint8_t flags = TYPE::NONE): _flags(flags) {}
    inline bool isSupported(TYPE type) const { return _flags & type; };
    inline bool isCompatible(const ResOperation& operation) const { return (_flags & operation._flags) == _flags; };
    inline bool isRead() const { return _flags & READ; }
    inline bool isWrite() const { return _flags & WRITE; }
    inline bool isExecute() const { return _flags & EXECUTE; }
    inline bool isDiscover() const { return _flags & DISCOVER; }
	inline bool isDelete() const { return _flags & DELETE; }
    inline bool isBlock1Write() const { return _flags & BLOCK1_WRITE; }
    inline bool isBlock1Execute() const { return _flags & BLOCK1_EXECUTE; }
    inline uint16_t getFlags() const { return _flags; }
    inline std::vector<TYPE> asVector() const {
    	uint8_t flags = _flags;
    	std::vector<TYPE> operations;
    	for (size_t i = 1; i != 0x80; i <<= 1) {
    		if (flags & i) operations.push_back((TYPE)i);
    	}
    	return operations;
    }

private:
    uint8_t _flags;
};

} // namespace wpp

#endif //WPP_RES_OPERATION_H
