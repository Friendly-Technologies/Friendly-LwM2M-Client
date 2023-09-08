#ifndef OPERATION_H
#define OPERATION_H

#include <vector>

#include "types.h"

namespace wpp {

#define MSB_IN_FLAG_VALUE (0x8000)

struct Operation {
public:
	enum TYPE: uint16_t {
        NONE = 0,
	    READ = 1,
	    WRITE = 2,
	    EXECUTE = 4,
	    DISCOVER = 8,
		CREATE = 16,
	    DELETE = 32,
	    BLOCK1_CREATE = 64,
	    BLOCK1_WRITE = 128,
	    BLOCK1_EXECUTE = 256,
	};

public:
	Operation(uint16_t flags = TYPE::NONE): _flags(flags) {}
    inline bool isSupported(TYPE type) const { return _flags & type; };
    inline bool isCompatible(const Operation& operation) const { return (_flags & operation._flags) == _flags; };
    inline bool isRead() const { return _flags & READ; }
    inline bool isWrite() const { return _flags & WRITE; }
    inline bool isCreate() const { return _flags & CREATE; }
    inline bool isDelete() const { return _flags & DELETE; }
    inline bool isExecute() const { return _flags & EXECUTE; }
    inline bool isDiscover() const { return _flags & DISCOVER; }
    inline bool isBlock1Create() const { return _flags & BLOCK1_CREATE; }
    inline bool isBlock1Write() const { return _flags & BLOCK1_WRITE; }
    inline bool isBlock1Execute() const { return _flags & BLOCK1_EXECUTE; }
    inline uint16_t getFlags() const { return _flags; }
    inline std::vector<TYPE> asVector() const {
    	uint16_t flags = _flags;
    	std::vector<TYPE> operations;

    	for (size_t i = 1; i != MSB_IN_FLAG_VALUE; i <<= 1) {
    		if (flags & i) operations.push_back((TYPE)i);
    	}

    	return operations;
    }

private:
    uint16_t _flags;
};

} // namespace wpp

#endif //OPERATION_H
