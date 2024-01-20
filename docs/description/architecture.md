\page architecture_tag Architecture Description

- [Overview](@ref arch_overview)
- [Components](@ref arch_components)
- [User Interfaces](@ref arch_user_interfaces)
- [State Management](@ref arch_client_state_management)
- [Registry Management](@ref arch_registry_management)
- [Object Management](@ref arch_object_management)
- [Platform Dependencies](@ref arch_platform_dependent)
- [Wpp Task Queue](@ref arch_wpp_task_queue)
- [Integration and Flexibility](@ref arch_integration_and_flexibility)
- [Sumary](@ref arch_summary)

### Overview {#arch_overview}
**WakaamaPlus**, also known as **Wpp**, is a project built upon the [**Wakaama**](https://github.com/eclipse/wakaama) project, which implements the **Lightweight Machine to Machine (LwM2M)** protocol. The architecture of **Wpp** is carefully designed to provide a high-level presentation of [**Wakaama**](https://github.com/eclipse/wakaama) functionalities while retaining control and flexibility.

### Components {#arch_components}
Since **Wpp** is built upon the [**Wakaama**](https://github.com/eclipse/wakaama), the existing interfaces and capabilities of [**Wakaama**](https://github.com/eclipse/wakaama) affect the internal structure of **Wpp**. For a better understanding of what components should be present in **Wpp**, the [**Wakaama**](https://github.com/eclipse/wakaama) interface was segregated, based on it, the necessary components for **Wpp** were created.

\image html images/wakaama_core_api_segregation.png width=950px

**Wpp's** architecture is composed of several key components, each serving a distinct role:

1. **State Management (wpp::WppClient)**: This component is responsible for the initialization of the [**Wakaama**](https://github.com/eclipse/wakaama) core, processing protocol states, managing server and object registrations.
   
2. **Object Model (wpp::Object wpp::ObjectSpec wpp::Instance wpp::Resource)**: This component is at the heart of **Wpp's** architecture. It defines the representation of objects at the **Wpp** level, their structure, general functionality, and sets the interface for interaction with instances and resources of the object, updating [**Wakaama**](https://github.com/eclipse/wakaama) about resource changes made by the user.

3. **Object Registry (wpp::WppRegistry)**: A crucial component that functions as a registry for available LwM2M objects. It manages the creation and initialization of objects and allows the user to create Instances and manage their resources.

4. **Connection (wpp::WppConnection)**: It defines the interface for establishing connections with the server and handling data transmission.

5. **Platform (wpp::WppPlatform wpp::WppGuard)**: This defines the interface for environment-dependent functions like memory allocation, logs printing, and time retrieval, and is entirely implemented by the user.

6. **Utils (folder utils)**: Includes various tools that is used by **Wpp**.

The diagram below shows the common **Wpp** components with their dependencies on [**Wakaama**](https://github.com/eclipse/wakaama). A part of the [**Wakaama**](https://github.com/eclipse/wakaama) library with its components is highlighted in **yellow**, and a part of the **Wpp** library is highlighted in **green**. The dependencies of the environment and the user program from the **Wpp** library are also indicated.

\image html images/wpp_dependencies_from_wakaama.jpg width=650px

### User Interfaces {#arch_user_interfaces}

The main interface of the library consists of two main parts:
- **Object registry management interface.** Which is used by the user to initialize the client, gain access to the object register, and manage objects. It consists from the next interfaces:
  - **State management interface.** Used by the user to initialize the client, get its state, access to registry, and manage registration. It consists from **wpp::WppClient** class which is implemented as singleton, and it is the main entry point through which interaction with the client takes place.
  - **Registry management interface.** Used by the user to gain access to the objects in the registry, register build-in objects, and get their state. It consists from **wpp::WppRegistry** class which is part of **wpp::WppClient**.
  - **Object management interface.** Used by the user to gain access to the objects state, instances, objects business logic, and internal data. It consists from **wpp::Object**, **wpp::ObjectSpec**, **wpp::Instance** classes, access to **wpp::Object** can be retreived through **wpp::WppRegistry** class.
- **Platform dependent interface.** It is used by the **Wpp** and must be implemented by the user or platform. It includes an interface
for establishing connection, data transfer, and platform-dependent functionality like: get time, print logs, resource access guard. It consists from the next classes: **wpp::WppConnection**, **wpp::WppPlatform**, **wpp::WppGuard**.

\image html images/wpp_relationship_and_structure.jpg width=650px

One more diagram.

\image html images/wpp_simplified_structure_with_deps.png width=950px

### State Management {#arch_client_state_management}

The **State Management** interface is presented in the form of a single class **wpp::WppClient**, which provides a single access point for processing the internal state of the client and the object registry. This approach allows to synchronize access to internal state changes when using the client by different threads. 
To create a client, the user must call the **wpp::WppClient::create()** method, as parameters this method accepts a structure with information about the client, through which user can set its name **wpp::WppClient::ClientInfo::endpointName**, the second parameter is an object of class **wpp::WppConnection** that is implemented by the user.

Synchronization of access to the client is provided by the ownership mechanism, the client can be owned by only one program subject at a time, for this purpose a pair of methods **wpp::WppClient::takeOwnership()** and **wpp::WppClient::giveOwnership()** are used. Any actions with the client can be performed only after the **wpp::WppClient::takeOwnership()** method returns a valid object. After completing the manipulations with the client, the owner **must** return control over the client by calling the **wpp::WppClient::giveOwnership()** method. **wpp::WppClient** has two types of methods to take ownership **wpp::WppClient::takeOwnership()** and **wpp::WppClient::takeOwnershipBlocking()**, the first method works without any blocking, if the client is already owned, it returns an empty pointer, but **wpp::WppClient::takeOwnershipBlocking()** works differently, if the client is owned, it blocks its call until the client is released after which takes ownership of the client and returns a valid object.

The second important part of **wpp::WppClient** is **wpp::WppClient::loop()** method for state processing, which when called performs three main tasks: processing packets received from the server, processing tasks from **wpp::WppTaskQueue**, handling the internal state of the [**Wakaama**](https://github.com/eclipse/wakaama) library. After completion, the **wpp::WppClient::loop()** method returns the maximum time in seconds after which the user must call this method the next time. But if after calling **wpp::WppClient::loop()** the user creates a task **wpp::WppTaskQueue** which must be called earlier than the next call to the method **wpp::WppClient::loop()** , then in this case, the **wpp::WppClient::loop()** method must be executed earlier than scheduled for the timely execution of the user request. It should be noted that the time returned after calling **wpp::WppClient::loop()** is the maximum amount of time after which this method should be executed, while the minimum threshold is not set. Therefore, the user can call this method even without delays between calls. It should also be noted that the next call is timed in seconds, so there is no need to call **wpp::WppClient::loop()** more often than once per second, although this is acceptable.

To access the registry, the client has a method **wpp::WppClient::registry()** that returns a registry object. Other methods and their purpose are described in the [Code](@ref code_tag) section at the following link **wpp::WppClient**.

### Registry Management {#arch_registry_management}

The **Registry management** interface consists of one class **wpp::WppRegistry**, which represents the registry of all existing objects in the client. Each object that exists in the client must be represented as a separate method in **wpp::WppRegistry**, for example, for the object **wpp::Device** there is a method **wpp::WppRegistry::device()** which when called, returns the corresponding object. User can also use another way to get the required object through **wpp::OBJ_ID**, for this needs to call the **wpp::WppRegistry::object()** method and pass the ID of the corresponding object. Each existing object in the client has its own unique **ID**, which is reflected in the **wpp::OBJ_ID** enumeration. In addition to the fact that **wpp::WppRegistry** stores all objects and provides access to them, it also has a function that allows to register the corresponding objects on the server, this is a pair of methods **wpp::WppRegistry::registerObj()** and **wpp::WppRegistry::deregisterObj()**, there are also methods that allows to get the current state of the object, more details at the following link **wpp::WppRegistry**. **wpp::WppRegistry** was designed in such a way that it can be used together with a set of utilities for generating objects [**object_maker**](../../utils/object_maker), so it is important not to disturb the internal structure and comments that exist in **wpp::WppRegistry**, and if it is necessary to do this, then it must be coordinated with utilities.

### Object Management {#arch_object_management}

The **Object Management** interface consists of four classes **wpp::Object**, **wpp::ObjectSpec**, **wpp::Instance**, **wpp::Resource**, but the user can interact with the interfaces of only three **wpp::Object**, **wpp::ObjectSpec**, **wpp::Instance**. The above classes represent the LwM2M data model of the object, which is described in the documentation [7.1. Resource Model](https://openmobilealliance.org/release/LightweightM2M/V1_1_1-20190617-A/OMA-TS-LightweightM2M_Core-V1_1_1-20190617-A.pdf).

The above classes create the following hierarchy:
1. **wpp::Object** is at the top level and defines the interface/behavior for LwM2M objects. Its main task is to manage instances (**wpp::Instance**) of the object and process requests from the server with subsequent transfer of these requests to the corresponding implementation of the object. In fact, **wpp::Object** is a container for instances of the **wpp::Instance** class, it has the following methods to manage instances: **wpp::Object::createInstance()** to create a new instance ( class instance **wpp::Instance**) by its **ID**, **wpp::Object::removeInstance()** to remove an instance by its **ID**, **wpp::Object: :instance()** to get the corresponding instance by its **ID**. **wpp::Object** also has a **wpp::Object::restore()** method that allows you to restore the initial state of the object, this method generates an event to the user, and the part responsible for servicing the corresponding object on the user's side, decides what initial state the object should have. A detailed description of all other methods can be found at the following link **wpp::Object**. Here it is worth telling how the user knows about the events that happen to the object, because in addition to the recovery event, there are others. Each instance of the **wpp::Object** class stored in the **wpp::WppRegistry** has a built-in monitoring mechanism via the **wpp::ObjSubject** class (**wpp::Object** inherits from **wpp::ObjSubject**), whereby **wpp::Object** receives the following methods **wpp::ObjSubject::opSubscribe()** and **wpp::ObjSubject::actSubscribe()**, these methods are used by the user to observe an object. **wpp::Object** has two types of events: an action request (used in the **wpp::Object::restore()** method), and an operation notification from the server (**wpp::InstOp::TYPE**). For example, if the server wants to create an object instance (**wpp::Instance**), then during this operation, if the user is subscribed to it, an event will be generated that will be sent to the user's observer. Since **wpp::Object** has two types of events, accordingly, the user has two types of observers **wpp::ObjActObserver** and **wpp::ObjOpObserver**, which are interfaces with a set of methods that must be implemented by the user. Each method corresponds to one of the events.

2. **wpp::ObjectSpec** 

3. **wpp::Instance**

### Wpp Task Queue {#arch_wpp_task_queue}

### Platform Dependencies {#arch_platform_dependent}

### Integration and Flexibility {#arch_integration_and_flexibility}
**Wpp's** architecture allows for easy integration into new environments and the possibility to implementation **Platform dependent interface** as per environmental requirements. This flexibility is attributed to the absence of external dependencies, as **Wpp** defines all input and output interfaces, and the environment implements them. For integration to new environment user should implement the next interfaces: **wpp::WppConnection**, **wpp::WppPlatform**, **wpp::WppGuard**. Example can be found in the [**platform**](../../examples/platform) folder.

### Summary {#arch_summary}
The architecture of **WakaamaPlus (Wpp)** is a well-structured framework that provides a robust platform for the development and implementation of LwM2M client applications. It maintains a balance between high-level functionality and control, and is designed to be flexible and adaptable to various user and environmental needs.



