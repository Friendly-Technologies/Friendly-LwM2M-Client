\page examples_tag Examples of Usage

This section covers examples of the use of the user interface and code generation utilities that were described in more detail in the [Architecture](@ref architecture_tag) section. A complete example of using **Wpp** is in the [**examples**](../../examples/) folder, it does not match the code in this section because the code in this section shows how to use **Wpp** and its interfaces.

- [State Management](@ref ex_client_state_management)
- [Registry Management](@ref ex_registry_management)
- [Object Management](@ref ex_object_management)
- [Platform Dependencies](@ref ex_platform_dependent)
- [Wpp Task Queue](@ref ex_wpp_task_queue)
- [Object Maker Tools](@ref ex_object_maker_tools)

### State Management {#ex_client_state_management}

Consider the initialization of **wpp::WppClient** and its use. Assume that the user has implemented the **wpp::WppPlatform**, **wpp::WppGuard**, **wpp::WppConnection** interfaces and has the corresponding classes **Platform**, **Guard**, **Connection**.

We include the header file of the library that describes the client. Then include the header file of the library that describes the registry. If necessary, make the wpp namespace available for the entire file.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;
\endcode

In the **main()** function, we initialize the client and start state processing.

We create an instance of the class that implements the **wpp::WppConnection** interface.
\code{.cpp}
Connection connection;
\endcode

We create the WppClient object and pass the structure with the client name "SinaiRnDTestLwm2m" and the connection object that will allow receiving and sending messages.
\code{.cpp}
WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
\endcode

At this stage, the client is created and ready to work, in order to make sure that the client has been initialized, we can use the **wpp::WppClient::isCreated()** method.
\code{.cpp}
if (WppClient::isCreated() == false) return -1;
\endcode

To use a client, we must first take ownership of it by calling **wpp::WppClient::takeOwnership()** or **wpp::WppClient::takeOwnershipBlocking()**.
\code{.cpp}
WppClient *client = WppClient::takeOwnershipBlocking();
\endcode

After successfully taking ownership, we can access the object registry, or get the current state of the client, for information on other functionality, see the **wpp::WppClient** class implementation.
\code{.cpp}
WppRegistry &registry = client->registry();
lwm2m_client_state_t clientState = client->getState();
\endcode

If the interaction with the client is complete, then control can be returned so that other program threads or interrupts can interact with it. But if the client will be used only from one context, then control over it can not be returned because this mechanism exists to synchronize simultaneous access to the client from different program contexts.
\code{.cpp}
client->giveOwnership();
\endcode

If the main configurations of the client are completed, then we can start processing the internal state, in the while loop.

We take ownership of the client, if we need to avoid blocking, we can use **wpp::WppClient::takeOwnership()**, but in this case we need to make sure that exactly the client object is returned.
\code{.cpp}
client = WppClient::takeOwnershipBlocking();
\endcode

We call the client's internal state handler, which processes incoming messages from the server, tasks, and the client's internal state. The **wpp::WppClient::loop()** method returns the number of seconds after which the handler needs to be called again, this is only needed so that the user can optimize their code, for example going to sleep mode. There are no limits on the maximum frequency of calls to the **wpp::WppClient::loop()** method.
\code{.cpp}
time_t sleepTimeSec = client->loop();
\endcode

We return control over the client so that other parts of the code can work with it.
\code{.cpp}
client->giveOwnership();
\endcode

We delay the execution process for the period of time specified by the client. But this code is written only as a test, because a delay for a long time is impractical in real systems, when a message arrives from the server, it is necessary to interrupt the delay so that the client can process it as quickly as possible, or when the user creates a new task that must be executed with zero delay. The optimal delay value is 1 second.
\code{.cpp}
delay(sleepTimeSec);
\endcode

After finishing work with **wpp::WppClient**, if necessary, can be called **wpp::WppClient::remove()** to release the resources occupied by the client and the registry.
\code{.cpp}
WppClient::remove();
\endcode

Will give a full example of the code in which the client can be used by several threads at the same time, but with blocking of the main cycle. By using the **wpp::WppClient::takeOwnershipBlocking()** method, which blocks execution until it takes ownership.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    
    WppClient *client = WppClient::takeOwnershipBlocking();
    WppRegistry &registry = client->registry();
    lwm2m_client_state_t clientState = client->getState();
    client->giveOwnership();
    
    while (true) {
        client = WppClient::takeOwnershipBlocking();
        time_t sleepTimeSec = client->loop();
        client->giveOwnership();
        some_other_code();
        // Just for example
        delay(sleepTimeSec);
    }

    WppClient::remove();
}
\endcode

An example of code in which the client can be used by several threads at the same time, without blocking the main loop. Because it uses **wpp::WppClient::takeOwnership()** which immediately returns **NULL** when the client take ownership fails.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    
    WppClient *client = WppClient::takeOwnershipBlocking();
    WppRegistry &registry = client->registry();
    lwm2m_client_state_t clientState = client->getState();
    client->giveOwnership();
    
    while (true) {
        client = WppClient::takeOwnership();
        if (client != NULL) {
            client->loop();
            client->giveOwnership();
        }
        some_other_code();
        // Just for example
        delay(1);
    }

    WppClient::remove();
}
\endcode

An example of code in which the client can be used by only one thread, so in this case there are no blocking.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    WppClient *client = WppClient::takeOwnershipBlocking();

    WppRegistry &registry = client->registry();
    lwm2m_client_state_t clientState = client->getState();
    
    while (true) {
        client->loop();
        some_other_code();
    }

    WppClient::remove();
}
\endcode

### Registry Management {#ex_registry_management}

In the section [State Management](@ref ex_client_state_management) an example of creating, initializing and using **wpp::WppClient** was considered. This section will consider the interface for accessing and interacting with the registry. As a basis, we will use the code from the [State Management](@ref ex_client_state_management) section, adding to it the **objects_registering()** method, which will interact with the registry.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

bool objects_registering(WppClient &client) {
    return true;
}

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    WppClient *client = WppClient::takeOwnershipBlocking();

    if (objects_registering(*client) == false) return -1;

    while (true) {
        client->loop();
        some_other_code();
    }

    WppClient::remove();
}
\endcode

According to [**Lightweight Machine to Machine Technical Specification**](https://www.openmobilealliance.org/release/LightweightM2M/V1_1_1-20190617-A/OMA-TS-LightweightM2M_Core-V1_1_1-20190617-A.pdf) objects can be of two types MANDATORY and OPTIONAL. MANDATORY objects are obliged to be present for the correct work of the client. In **wpp::WppRegistry**, this division is conditional, the only feature inherent in MANDATORY objects is that they are immediately registered in the client after creating **wpp::WppClient**, that is, for MANDATORY objects you do not need to separately call **wpp::WppRegistry::registerObj()**, but this behavior applies only to the three MANDATORY objects **wpp::Device**, **wpp::Lwm2mServer** and **wpp::Lwm2mSecurity**. The registry allows to disable and enable the necessary objects at the compilation stage to save client resources, this is done through the [**objects_config.cmake**](../../wpp/configs/objects_config.cmake) configuration file. The integration of new objects into the registry is also supported due to the set of utilities [**object_maker**](../../utils/object_maker).

For example, in the file [**objects_config.cmake**](../../wpp/configs/objects_config.cmake) support for six objects was enabled: **wpp::Device**, **wpp::Lwm2mServer**, **wpp::Lwm2mSecurity**, **wpp::ConnectivityMonitoring**, **wpp::Lwm2mAccessControl**, **wpp::FirmwareUpdate** corresponding configurations.

Consider an example of using the **wpp::WppRegistry** interface. We will release the **objects_registering()** method. First, we will get access to the register of objects.
\code{.cpp}
WppRegistry &registry = client.registry();
\endcode

Objects **wpp::Device**, **wpp::Lwm2mServer** and **wpp::Lwm2mSecurity** are already registered, so it remains to register **wpp::ConnectivityMonitoring**, **wpp::Lwm2mAccessControl**, **wpp::FirmwareUpdate**. Registration of objects is necessary for the client to inform the server of their presence, since the client sends the server a list of all registered objects, not existing ones. Before registering, we should make sure that the objects exist, this can be done using the **wpp::WppRegistry::isObjExist()** method.
\code{.cpp}
if (registry.isObjExist(OBJ_ID::CONNECTIVITY_MONITORING) == false) return false;
if (registry.isObjExist(OBJ_ID::LWM2M_ACCESS_CONTROL) == false) return false;
if (registry.isObjExist(OBJ_ID::FIRMWARE_UPDATE) == false) return false;
\endcode

To register objects in the **wpp::WppRegistry::registerObj()** method, we need to pass the inheritor of the **wpp::Object** class. There are two ways to get **wpp::Object**: through a specialized method that always returns an object of the same type such as **wpp::WppRegistry::device()**, or through **wpp::WppRegistry::object()** which returns a **wpp::Object** with the specified **wpp::OBJ_ID**. The first approach exists for using a specialized interface specific to a specific object, the other for using an object through a generalized interface.
\code{.cpp}
Object *connMon = &registry.connectivityMonitoring();
Object *acl = &registry.lwm2mAccessControl();
Object *fwUpd = registry.object(OBJ_ID::FIRMWARE_UPDATE);
\endcode

We perform the registration of the necessary objects.
\code{.cpp}
registry.registerObj(*connMon);
registry.registerObj(*acl);
registry.registerObj(*fwUpd);
\endcode

After registration, we can check whether the objects are registered by calling the **wpp::WppRegistry::isObjRegistered()** method.
\code{.cpp}
if (registry.isObjRegistered(*connMon) == false) return false;
if (registry.isObjRegistered(*acl) == false) return false;
if (registry.isObjRegistered(*fwUpd) == false) return false;
\endcode

So this is the entire core interface for wpp::WppRegistry, it allows to register objects and access them. Complete code example.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

bool objects_registering(WppClient &client) {
    WppRegistry &registry = client.registry();

    if (registry.isObjExist(OBJ_ID::CONNECTIVITY_MONITORING) == false) return false;
    if (registry.isObjExist(OBJ_ID::LWM2M_ACCESS_CONTROL) == false) return false;
    if (registry.isObjExist(OBJ_ID::FIRMWARE_UPDATE) == false) return false;

    Object *connMon = &registry.connectivityMonitoring();
    Object *acl = &registry.lwm2mAccessControl();
    Object *fwUpd = registry.object(OBJ_ID::FIRMWARE_UPDATE);

    registry.registerObj(*connMon);
    registry.registerObj(*acl);
    registry.registerObj(*fwUpd);

    if (registry.isObjRegistered(*connMon) == false) return false;
    if (registry.isObjRegistered(*acl) == false) return false;
    if (registry.isObjRegistered(*fwUpd) == false) return false;

    return true;
}

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    WppClient *client = WppClient::takeOwnershipBlocking();

    if (objects_registering(*client) == false) return -1;

    while (true) {
        client->loop();
        some_other_code();
    }

    WppClient::remove();
}
\endcode

### Object Management {#ex_object_management}

In the previous sections, we discussed creating a client and registering objects, but objects themselves have no value because all information about the state of an object is stored in its instances. Therefore, this section will consider the creation of object instances, their initialization, and use. As a basis, we will take the code from the previous section and add to it three functions **device_init()**, **server_init** and **security_init()**, which will contain the initialization of three relevant objects, thanks to which the example will meet the minimum requirements under which the client will be able to join the LwM2M server.

The method of monitoring objects and their instances will also be considered, thanks to which the user will be able to receive information about the necessary events.

Since an initialization example will be given for **wpp::Device**, **wpp::Lwm2mServer** and **wpp::Lwm2mSecurity**, which by default are registered in the client, the **objects_registering()** method can be removed from the code example. The initial version of the code will look like this.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

void device_init(WppClient &client) {}
void server_init(WppClient &client) {}
void security_init(WppClient &client) {}
void read_device_data(WppClient &client) {}

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    WppClient *client = WppClient::takeOwnershipBlocking();

    device_init(*client);
    server_init(*client);
    security_init(*client);
    read_device_data(*client);

    while (true) {
        client->loop();
        some_other_code();
    }

    WppClient::remove();
}
\endcode

Let's start by initializing the **wpp::Device** object and create an implementation for the **device_init()** function.

We get the register of objects from which we extract the **wpp::Device** object.
\code{.cpp}
Object *deviceObj = client.registry().object(OBJ_ID::DEVICE);
\endcode

We create an instance of the **wpp::Device** object. To do this, we need to call the **wpp::Object::createInstance()** method and pass the instance ID as a parameter, if the ID is not important, then it can be omitted, in which case the first available value will be taken.
\code{.cpp}
Instance *deviceInst = deviceObj->createInstance();
\endcode

It should be noted that there are two ways of accessing objects: specialized and generalized. The generalized interface was given above, but if you need to access a specific interface inherent only to the **wpp::Device** object, then you should use a specialized method.
\code{.cpp}
ObjectSpec<Device> &deviceObj = client.registry().device();
Device *deviceInst = deviceObj.createInstanceSpec();
\endcode

If the object instance has already been created and you need to access it, you can use one of the following methods: **wpp::Object::instance()** or **wpp::ObjectSpec<T>::instanceSpec()**.
\code{.cpp}
Instance *deviceInst = client.registry().object(OBJ_ID::DEVICE).instance(0);
Device *deviceInst = client.registry().device().instanceSpec(0);
\endcode

After creating the instance, we can proceed to its initialization. First let's clarify that according to [**Lightweight Machine to Machine Technical Specification**](https://www.openmobilealliance.org/release/LightweightM2M/V1_1_1-20190617-A/OMA-TS-LightweightM2M_Core-V1_1_1-20190617-A .pdf) there are three types of operations that can be performed by the server on the data of the object instance: writing, reading and execution, but only writing and reading can be performed from the user's side, since the behavior when performed is specified by the user, and there is no point in duplicating the behavior from the user's side.

To set the value, there are two types of methods **wpp::Instance::set()** and **wpp::Instance::setMove()** the first creates two copies in memory, the second moves the data from the user change to the instance. A similar situation with obtaining a value is two methods **wpp::Instance::get()** and **wpp::Instance::getPtr()**, the first returns a copy of the data, the second a constant pointer to the data stored in the instance, which means that the user will not be able to change them. When writing or reading data, two parameters are passed to the corresponding method, the first indicates the ID of the resource to which it is necessary to write or from which it is necessary to read, the second parameter the user variable from which the value for writing will be taken, or to which the value of the resource will be written. Data is stored as resources inside the object instance. Each resource has its own unique ID, which is the same for all instances of the object. The purpose and characteristics of each resource in the object are standardized and described [**here**](https://technical.openmobilealliance.org/OMNA/LwM2M/LwM2MRegistry.html).

Let's consider the most important characteristics:
1. A resource has only one type of data throughout its existence. Data types are standardized and have their representation in **Wpp**: **STRING_T** (String), **INT_T** (Integer), **UINT_T** (Unsigned Integer), **FLOAT_T** (Float) , **BOOL_T** (Boolean), **OPAQUE_T** (Opaque), **TIME_T** (Time), **OBJ_LINK_T** (Objlnk), **CORE_LINK_T** (Corelnk), **EXECUTE_T** (none). The data type of the value being set or read must always be one of the above.
2. A resource can contain several values of the same type (as an array), in which case the resource is considered **MULTIPLE**, and each element of this resource is called a **resource instance**. Each resource instance has its own unique ID within this resource. Therefore, if you need to refer to a resource instance, then in the corresponding method you need to specify two values, the first will indicate the ID of the resource itself, the other will indicate the ID of the resource instance.
3. Resources may have certain restrictions regarding the values they store, these restrictions are described by the standard and reflected in the object implementations.

Let's initialize the **wpp::Device** object, each object implementation has a list of available resources in the form of an enum for easier access.
\code{.cpp}
deviceInst->set(Device::ERROR_CODE_11, (INT_T)Device::NO_ERROR);
deviceInst->set(Device::SUPPORTED_BINDING_AND_MODES_16, WPP_BINDING_UDP);
deviceInst->set(Device::MANUFACTURER_0, (STRING_T)"Wakaama Plus");
deviceInst->set(Device::MODEL_NUMBER_1, (STRING_T)"Lightweight M2M Client");
deviceInst->set(Device::SERIAL_NUMBER_2, (STRING_T)"0123456789");
\endcode

If the resource has type **EXECUTE_T**, then this resource is functional, the function bound to it will be executed when the server applies the execute operation to the specified resource. For example, let's set a function that must be executed when the server wants to reboot the device.
\code{.cpp}
deviceInst->set(Device::REBOOT_4, (EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) {
    requestRebootForDevice();
    return true;
});
\endcode

Objects are not required to implement all resources, only the main ones that are marked as MANDATORY in the documentation, so the implementation of the object is made in such a way that it allows turning on and off the necessary resources through the definition, which will reduce memory consumption. Each implementation of the object contains a file **object_name+Config.h** through which you can enable or disable the required resource, the file for the object can be found at the following path **wpp/registry/objects/object_name/object_name+Config.h**.

The complete version of the code for initializing the **wpp::Device** object.
\code{.cpp}
void device_init(WppClient &client) {
    Object *deviceObj = client.registry().object(OBJ_ID::DEVICE);
    Instance *deviceInst = deviceObj->createInstance();

    deviceInst->set({Device::ERROR_CODE_11, 0}, (INT_T)Device::NO_ERROR);
    deviceInst->set(Device::SUPPORTED_BINDING_AND_MODES_16, WPP_BINDING_UDP);
    deviceInst->set(Device::MANUFACTURER_0, (STRING_T)"Wakaama Plus");
    deviceInst->set(Device::MODEL_NUMBER_1, (STRING_T)"Lightweight M2M Client");
    deviceInst->set(Device::SERIAL_NUMBER_2, (STRING_T)"0123456789");
    deviceInst->set(Device::REBOOT_4, (EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) {
        requestRebootForDevice();
        return true;
    });
}
\endcode

For the **wpp::Lwm2mServer** object.
\code{.cpp}
void server_init(WppClient &client) {
    ObjectSpec<Lwm2mServer> &serverObj = client.registry().lwm2mServer();
    Lwm2mServer *serverInst = serverObj.createInstanceSpec();

	serverInst->set(Lwm2mServer::SHORT_SERVER_ID_0, INT_T(123));
	serverInst->set(Lwm2mServer::BINDING_7, WPP_BINDING_UDP);
	serverInst->set(Lwm2mServer::LIFETIME_1, TIME_T(25));
	serverInst->set(Lwm2mServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);
}
\endcode

For the **wpp::Lwm2mSecurity** object.
\code{.cpp}
void security_init(WppClient &client) {
    Object *securityObj = &client.registry().lwm2mSecurity();
    Instance *securityInst = securityObj->createInstance();

    securityInst->set(Lwm2mSecurity::SECURITY_MODE_2, (INT_T)LWM2M_SECURITY_MODE_NONE);
    securityInst->set(Lwm2mSecurity::BOOTSTRAP_SERVER_1, false);
    securityInst->set(Lwm2mSecurity::SHORT_SERVER_ID_10, (INT_T)123);

    STRING_T uri = "coap://demodm.friendly-tech.com:5683";
    securityInst->setMove(Lwm2mSecurity::LWM2M_SERVER_URI_0, uri);
}
\endcode

Let's give an example of the code for getting data from an object.
\code{.cpp}
void read_device_data(WppClient &client) {
    Instance *deviceInst = client.registry().device().instance();

    INT_T errorCode;
    STRING_T binding;
    STRING_T manufacture;

    deviceInst->get({Device::ERROR_CODE_11, 0}, errorCode);
    deviceInst->get(Device::SUPPORTED_BINDING_AND_MODES_16, binding);
    deviceInst->get(Device::MANUFACTURER_0, manufacture);

    const STRING_T *model;
    deviceInst->getPtr(Device::MODEL_NUMBER_1, &model);
}
\endcode

Consider the interface for monitoring objects and instances. First, consider the method of receiving events from **wpp::Object**. To observe objects, it is necessary to implement one of the interfaces: **wpp::ObjActObserver**, **wpp::ObjOpObserver**, and subscribe to the observation by calling the corresponding method **wpp::ObjSubject::opSubscribe()** or **wpp::ObjSubject::actSubscribe()**. **wpp::ObjActObserver** allows to receive information about events that may require certain actions from the owner of the object, currently there is only one action, a request to restore the initial state of the object, upon receiving this request, the user can restore the initial state of the object if necessary. **wpp::ObjOpObserver** are informational events that inform the user about operations performed on the object from the server side. Currently, there are two events, about the creation of an object instance and its deletion.

Consider an example of creating an observer and registering it. Let's create an **ObjectObserver** class that will inherit from **wpp::ObjActObserver** and **wpp::ObjOpObserver**.
\code{.cpp}
class ObjectObserver : public ObjActObserver, public ObjOpObserver {
    void objectRestore(Object &object) override {}
    void instanceCreated(Object &object, ID_T instanceId) override {}
    void instanceDeleting(Object &object, ID_T instanceId) override {}
};
// Creating global observer for all object
static ObjectObserver objObserver; 
\endcode

Let's update the implementation of the **device_init()**, **server_init()**, **security_init()** functions by adding the **objObserver** observer registration to them.
\code{.cpp}
...
deviceObj->opSubscribe(&objObserver);
deviceObj->actSubscribe(&objObserver);
...
serverObj.opSubscribe(&objObserver);
serverObj.actSubscribe(&objObserver);
...
securityObj->opSubscribe(&objObserver);
securityObj->actSubscribe(&objObserver);
...
\endcode

A similar approach is also implemented for monitoring **wpp::Instance** events. To observe instances, it is necessary to implement one of the interfaces: **wpp::InstOpObserver**, **wpp::InstEventObserver**, and subscribe to the observation by calling the corresponding method **wpp::InstSubject::opSubscribe()** or **wpp::InstSubject::eventSubscribe()**. **wpp::InstOpObserver** are informational events that notify the user about operations performed on the instance from the server side. Currently, there are four events: writing to the resource, reading, executing the resource, completely replacing one of the instances (in fact, the old instance is deleted and a new one is substituted in its place, but the observer remains valid). **wpp::InstEventObserver** allows the user to receive information about specific events of the object inherent in the implementation of the instance.

Will give an example of creating an observer and registering it. Let's create an **InstanceObserver** class that will inherit from **wpp::InstOpObserver** and **wpp::InstEventObserver**.
\code{.cpp}
class InstanceObserver : public InstEventObserver, public InstOpObserver {
    void instEvent(Instance &inst, EVENT_ID_T eventId) override {}
    void resourceRead(Instance &inst, const ResLink &resource) override {}
    void resourceWrite(Instance &inst, const ResLink &resource) override {}
    void resourceExecute(Instance &inst, const ResLink &resource) override {}
    void resourcesReplaced(Instance &inst) override {}
};
// Creating global observer for all object
static InstanceObserver instObserver; 
\endcode

Let's update the implementation of **device_init()**, **server_init()**, **security_init()** functions by adding **instObserver** observer registration to them.
\code{.cpp}
...
deviceInst->opSubscribe(&instObserver);
deviceInst->eventSubscribe(&instObserver);
...
serverInst->opSubscribe(&instObserver);
serverInst->eventSubscribe(&instObserver);
...
securityInst->opSubscribe(&instObserver);
securityInst->eventSubscribe(&instObserver);
...
\endcode

The full version of the code that gives an example of obtaining access to objects, creating their instances and registering observers for objects and instances.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

class ObjectObserver : public ObjActObserver, public ObjOpObserver {
    void objectRestore(Object &object) override {}
    void instanceCreated(Object &object, ID_T instanceId) override {}
    void instanceDeleting(Object &object, ID_T instanceId) override {}
};
// Creating global observer for all object
static ObjectObserver objObserver; 

class InstanceObserver : public InstEventObserver, public InstOpObserver {
    void instEvent(Instance &inst, EVENT_ID_T eventId) override {}
    void resourceRead(Instance &inst, const ResLink &resource) override {}
    void resourceWrite(Instance &inst, const ResLink &resource) override {}
    void resourceExecute(Instance &inst, const ResLink &resource) override {}
    void resourcesReplaced(Instance &inst) override {}
};
// Creating global observer for all object
static InstanceObserver instObserver; 

void device_init(WppClient &client) {
    Object *deviceObj = client.registry().object(OBJ_ID::DEVICE);
    deviceObj->opSubscribe(&objObserver);
    deviceObj->actSubscribe(&objObserver);

    Instance *deviceInst = deviceObj->createInstance();
    deviceInst->opSubscribe(&instObserver);
    deviceInst->eventSubscribe(&instObserver);

    deviceInst->set({Device::ERROR_CODE_11, 0}, (INT_T)Device::NO_ERROR);
    deviceInst->set(Device::SUPPORTED_BINDING_AND_MODES_16, WPP_BINDING_UDP);
    deviceInst->set(Device::MANUFACTURER_0, (STRING_T)"Wakaama Plus");
    deviceInst->set(Device::MODEL_NUMBER_1, (STRING_T)"Lightweight M2M Client");
    deviceInst->set(Device::SERIAL_NUMBER_2, (STRING_T)"0123456789");
    deviceInst->set(Device::REBOOT_4, (EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) {
        requestRebootForDevice();
        return true;
    });
}

void server_init(WppClient &client) {
    ObjectSpec<Lwm2mServer> &serverObj = client.registry().lwm2mServer();
    serverObj.opSubscribe(&objObserver);
    serverObj.actSubscribe(&objObserver);

    Lwm2mServer *serverInst = serverObj.createInstanceSpec();
    serverInst->opSubscribe(&instObserver);
    serverInst->eventSubscribe(&instObserver);

	serverInst->set(Lwm2mServer::SHORT_SERVER_ID_0, INT_T(123));
	serverInst->set(Lwm2mServer::BINDING_7, WPP_BINDING_UDP);
	serverInst->set(Lwm2mServer::LIFETIME_1, TIME_T(25));
	serverInst->set(Lwm2mServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);
}

void security_init(WppClient &client) {
    Object *securityObj = &client.registry().lwm2mSecurity();
    securityObj->opSubscribe(&objObserver);
    securityObj->actSubscribe(&objObserver);

    Instance *securityInst = securityObj->createInstance();
    securityInst->opSubscribe(&instObserver);
    securityInst->eventSubscribe(&instObserver);

    securityInst->set(Lwm2mSecurity::SECURITY_MODE_2, (INT_T)LWM2M_SECURITY_MODE_NONE);
    securityInst->set(Lwm2mSecurity::BOOTSTRAP_SERVER_1, false);
    securityInst->set(Lwm2mSecurity::SHORT_SERVER_ID_10, (INT_T)123);

    STRING_T uri = "coap://demodm.friendly-tech.com:5683";
    securityInst->setMove(Lwm2mSecurity::LWM2M_SERVER_URI_0, uri);
}

void read_device_data(WppClient &client) {
    Instance *deviceInst = client.registry().device().instance();

    INT_T errorCode;
    STRING_T binding;
    STRING_T manufacture;

    deviceInst->get({Device::ERROR_CODE_11, 0}, errorCode);
    deviceInst->get(Device::SUPPORTED_BINDING_AND_MODES_16, binding);
    deviceInst->get(Device::MANUFACTURER_0, manufacture);

    const STRING_T *model;
    deviceInst->getPtr(Device::MODEL_NUMBER_1, &model);
}

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    WppClient *client = WppClient::takeOwnershipBlocking();

    device_init(*client);
    server_init(*client);
    security_init(*client);
    read_device_data(*client);

    while (true) {
        client->loop();
        some_other_code();
        delaySec(1);
    }

    WppClient::remove();
}
\endcode

### Platform Dependencies {#ex_platform_dependent}

**Wpp**  has a part that depends on the platform on which the library is running. This part is presented in the form of three interfaces **wpp::WppPlatform**, **wpp::WppGuard** and **wpp::WppConnection**, which must be implemented by the user. To get more information about their purpose, you can go to the corresponding part with a description of the code, and to get an implementation example, go to the [**platform**](../../examples/platform) folder.

### Wpp Task Queue {#ex_wpp_task_queue}

**Wpp** has an internal simplified task scheduler that runs in the context of the library and processes existing tasks when calling **wpp::WppClient::loop()**. Its work features are described in more detail [**here**](@ref arch_wpp_task_queue). It is used simultaneously for the internal needs of the libraries, such as updating the time in the resource of the Device object, and for the needs of the user. That is, the user can create his own task that will be performed in the context of the library, once or during a certain period of time, a more detailed description of the interface can be found at the following link **wpp::WppTaskQueue**.

Let's give an example of creating a task that will be executed only once in 5 seconds.
\code{.cpp}
WppTaskQueue::addTask(5, [](WppClient &client, void *ctx) -> bool {
    std::cout << "Wpp user task 1" << std::endl;
    return true;
});
\endcode

An example of a task that will be performed continuously with an interval of 2 seconds.
\code{.cpp}
WppTaskQueue::addTask(2, [](WppClient &client, void *ctx) -> bool {
    std::cout << "Wpp user task 2" << std::endl;
    return false;
});
\endcode

An example of a task that will be performed 3 times with an interval of 10 seconds.
\code{.cpp}
WppTaskQueue::addTask(10, [](WppClient &client, void *ctx) -> bool {
    static int cnt = 0;
    std::cout << "Wpp user task 3" << std::endl;
    cnt++;
    return cnt == 3;
});
\endcode

If necessary, the context may be passed to the task by reference or by copying.
Let's give an example of creating a task that will be executed only once in 3 seconds and pass a link to the data to it.
\code{.cpp}
STRING_T data1 = "Test string for task 4!";
WppTaskQueue::addTask(&data1, 5, [](WppClient &client, void *ctx) -> bool {
    std::cout << "Wpp user task 4, data: " << *((STRING_T *)ctx) << std::endl;
    return true;
});
\endcode

Let's give an example of creating a task that will be executed only once after 4 seconds and transfer a copy of the data to it.
\code{.cpp}
STRING_T data2 = "Test string for task 5!";
WppTaskQueue::addTaskWithCopy(data2.c_str(), 5, [](WppClient &client, void *ctx) -> bool {
    std::cout << "Wpp user task 5, data: " << (char *)ctx << std::endl;
    return true;
});
\endcode


The whole example together.
\code{.cpp}
#include "WppClient.h"
#include "WppRegistry.h"
using namespace wpp;

...

int main() {
    Connection connection;
    
    WppClient::create({"SinaiRnDTestLwm2m", "", ""}, connection);
    if (WppClient::isCreated() == false) return -1;
    WppClient *client = WppClient::takeOwnershipBlocking();

    device_init(*client);
    server_init(*client);
    security_init(*client);
    read_device_data(*client);

    WppTaskQueue::addTask(5, [](WppClient &client, void *ctx) -> bool {
        std::cout << "Wpp user task 1" << std::endl;
        return true;
    });
    WppTaskQueue::addTask(2, [](WppClient &client, void *ctx) -> bool {
        std::cout << "Wpp user task 2" << std::endl;
        return false;
    });
    WppTaskQueue::addTask(10, [](WppClient &client, void *ctx) -> bool {
        static int cnt = 0;
        std::cout << "Wpp user task 3" << std::endl;
        cnt++;
        return cnt == 3;
    });
    STRING_T data1 = "Test string for task 4!";
    WppTaskQueue::addTask(&data1, 5, [](WppClient &client, void *ctx) -> bool {
        std::cout << "Wpp user task 4, data: " << *((STRING_T *)ctx) << std::endl;
        return true;
    });
    STRING_T data2 = "Test string for task 5!";
    WppTaskQueue::addTaskWithCopy(data2.c_str(), data2.size() , 5, [](WppClient &client, void *ctx) -> bool {
        std::cout << "Wpp user task 5, data: " << (char *)ctx << std::endl;
        return true;
    });
    while (true) {
        client->loop();
        some_other_code();
        delaySec(1);
    }

    WppClient::remove();
}
\endcode

### Object Maker Tools {#ex_object_maker_tools}
