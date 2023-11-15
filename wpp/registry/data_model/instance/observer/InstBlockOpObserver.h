#ifndef WPP_INST_BLOCK_OP_OBSERVER_H_
#define WPP_INST_BLOCK_OP_OBSERVER_H_

#include "types.h" 

namespace wpp {

class Instance;

class InstBlockOpObserver {
public:
    virtual ~InstBlockOpObserver() {};
    /*
     * Notifies the observer about the block write operation and
     * its parameters. The resource during this operation is not
     * automatically writes, all data is transferred in the 
     * form of parameters.
     */
    virtual void resourceBlockWrite(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {};
    /*
     * Notifies the observer about the execution block operation
     * and its parameters. The resource during this operation is
     * not execute automatically, all data is transferred in the
     * form of parameters.
     */
    virtual void resourceBlockExecute(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {};
};

} // namespace wpp

#endif /* WPP_INST_BLOCK_OP_OBSERVER_H_ */