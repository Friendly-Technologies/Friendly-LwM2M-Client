#ifndef WPP_INST_OPERATION_H
#define WPP_INST_OPERATION_H

#include <vector>

#include "types.h"

namespace wpp {

struct InstOp {
public:
	enum TYPE: uint8_t {
		NONE = 0,
		CREATE = 1,
	    DELETE = 2,
	    BLOCK1_CREATE = 4,
	};

public:
	InstOp(uint8_t flags = TYPE::NONE): _flags(flags) {}
    inline bool isSupported(TYPE type) const { return _flags & type; };
    inline bool isCompatible(const InstOp& operation) const { return (_flags & operation._flags) == _flags; };
    inline bool isCreate() const { return _flags & CREATE; }
    inline bool isDelete() const { return _flags & DELETE; }
    inline bool isBlock1Create() const { return _flags & BLOCK1_CREATE; }
    inline uint8_t getFlags() const { return _flags; }
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

#endif //WPP_INST_OPERATION_H
