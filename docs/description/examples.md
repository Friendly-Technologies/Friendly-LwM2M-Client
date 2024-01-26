\page examples_tag Examples of Usage

This section covers examples of the use of the user interface and code generation utilities that were described in more detail in the [Architecture](@ref architecture_tag) section.

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
### Platform Dependencies {#ex_platform_dependent}
### Wpp Task Queue {#ex_wpp_task_queue}
### Object Maker Tools {#ex_object_maker_tools}