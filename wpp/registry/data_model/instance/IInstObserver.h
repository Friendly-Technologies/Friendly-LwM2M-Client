#ifndef IINSTOBSERVER_H_
#define IINSTOBSERVER_H_

namespace wpp {

template <typename T>
class IInstObserver {
public:
    virtual ~IInstObserver() {};
    /*
     * Notifies observer immediately after read resource by server. 
     * Resource is already readed during this call.
     */
    virtual void resourceRead(T &inst, const ResourceID &resource) = 0;
    /*
     * Notifies observer immediately after write resource by server. 
     * Resource is already writed during this call.
     */
    virtual void resourceWrite(T &inst, const ResourceID &resource) = 0;
    /*
     * Notifies observer immediately after execute resource by server. 
     * Resource is already executed during this call.
     */
    virtual void resourceExecute(T &inst, const ResourceID &resource) = 0;
};

} // namespace wpp

#endif /* IINSTOBSERVER_H_ */