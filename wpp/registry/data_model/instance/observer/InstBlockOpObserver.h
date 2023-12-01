#ifndef WPP_INST_BLOCK_OP_OBSERVER_H_
#define WPP_INST_BLOCK_OP_OBSERVER_H_

#include "types.h" 

namespace wpp {

class Instance;

/**
 * @brief The InstBlockOpObserver class is an abstract base class that defines the interface for observing instance block operations.
 * 
 * This class provides a set of virtual functions that can be overridden to receive notifications about instance block operations.
 * It is intended to be subclassed by concrete observer classes that are interested in monitoring the block operations of
 * a specific instance.
 */
class InstBlockOpObserver {
public:
    virtual ~InstBlockOpObserver() {};
    /**
     * @brief Notifies the observer about the block write operation and
     * its parameters. The resource during this operation is not
     * automatically writes, all data is transferred in the 
     * form of parameters.
     */
    virtual void resourceBlockWrite(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {};
    /**
     * @brief Notifies the observer about the execution block operation
     * and its parameters. The resource during this operation is
     * not execute automatically, all data is transferred in the
     * form of parameters.
     */
    virtual void resourceBlockExecute(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {};
};

} // namespace wpp

#endif /* WPP_INST_BLOCK_OP_OBSERVER_H_ */