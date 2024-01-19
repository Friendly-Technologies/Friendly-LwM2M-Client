\page architecture_tag Architecture Description

- [Overview](@ref arch_overview)
- [Components](@ref arch_components)
- [User Interfaces](@ref arch_user_interfaces)
- [Integration and Flexibility](@ref arch_integration_and_flexibility)
- [Sumary](@ref arch_summary)

### Overview {#arch_overview}
**WakaamaPlus**, also known as **Wpp**, is a project built upon the [**Wakaama**](https://github.com/eclipse/wakaama) project, which implements the **Lightweight Machine to Machine (LwM2M)** protocol. The architecture of **Wpp** is carefully designed to provide a high-level presentation of [**Wakaama**](https://github.com/eclipse/wakaama) functionalities while retaining control and flexibility.

### Components {#arch_components}
Since **Wpp** is built upon the [**Wakaama**](https://github.com/eclipse/wakaama), the existing interfaces and capabilities of [**Wakaama**](https://github.com/eclipse/wakaama) affect the internal structure of **Wpp**. For a better understanding of what components should be present in **Wpp**, the [**Wakaama**](https://github.com/eclipse/wakaama) interface was segregated, based on it, the necessary components for **Wpp** were created.

\image html images/wakaama_core_api_segregation.png width=950px

**Wpp's** architecture is composed of several key components, each serving a distinct role:

1. **State Management (wpp::WppClient)**: This component is responsible for the initialization of the [**Wakaama**](https://github.com/eclipse/wakaama) core, processing protocol states, managing server and object registrations.
   
2. **Object Model (wpp::Object wpp::Instance wpp::Resource)**: This component is at the heart of **Wpp's** architecture. It defines the representation of objects at the **Wpp** level, their structure, general functionality, and sets the interface for interaction with instances and resources of the object, updating [**Wakaama**](https://github.com/eclipse/wakaama) about resource changes made by the user.

3. **Object Registry (wpp::WppRegistry)**: A crucial component that functions as a registry for available LwM2M objects. It manages the creation and initialization of objects and allows the user to create Instances and manage their resources.

4. **Connection (wpp::WppConnection)**: It defines the interface for establishing connections with the server and handling data transmission.

5. **Platform (wpp::WppPlatform wpp::WppGuard)**: This defines the interface for environment-dependent functions like memory allocation, logs printing, and time retrieval, and is entirely implemented by the user.

6. **Utils (folder utils)**: Includes various tools that is used by **Wpp**.

The diagram below shows the common **Wpp** components with their dependencies on [**Wakaama**](https://github.com/eclipse/wakaama). A part of the [**Wakaama**](https://github.com/eclipse/wakaama) library with its components is highlighted in **yellow**, and a part of the **Wpp** library is highlighted in **green**. The dependencies of the environment and the user program from the **Wpp** library are also indicated.

\image html images/wpp_dependencies_from_wakaama.jpg width=650px

### User Interfaces {#arch_user_interfaces}

The main interface of the library consists of two main parts:
- **Object registry management interface.** Which is used by the user to initialize the client, gain access to the object register, and manage objects. It consists from the next interfaces:
  - **Client state management interface.** Used by the user to initialize the client, get its state, access to registry, and manage registration. It consists from **wpp::WppClient** class which is implemented as singleton, and it is the main entry point through which interaction with the client takes place.
  - **Registry management interface.** Used by the user to gain access to the objects in the registry, register build-in objects, and get their state. It consists from **wpp::WppRegistry** class which is part of **wpp::WppClient**.
  - **Object management interface.** Used by the user to gain access to the objects state, instances, objects business logic, and internal data. It consists from **wpp::Object**, **wpp::Instance** classes, access to **wpp::Object** can be retreived through **wpp::WppRegistry** class.
- **Platform dependent interface.** It is used by the **Wpp** and must be implemented by the user or platform. It includes an interface
for establishing connection, data transfer, and platform-dependent functionality like: get time, print logs, resource access guard. It consists from the next classes: **wpp::WppConnection**, **wpp::WppPlatform**, **wpp::WppGuard**.

\image html images/wpp_relationship_and_structure.jpg width=650px

One more diagram.

\image html images/wpp_simplified_structure_with_deps.png width=950px

### Integration and Flexibility {#arch_integration_and_flexibility}
**Wpp's** architecture allows for easy integration into new environments and the possibility to implementation **Platform dependent interface** as per environmental requirements. This flexibility is attributed to the absence of external dependencies, as **Wpp** defines all input and output interfaces, and the environment implements them. For integration to new environment user should implement the next interfaces: **wpp::WppConnection**, **wpp::WppPlatform**, **wpp::WppGuard**. Example can be found in the [**platform**](../../examples/platform) folder.

### Summary {#arch_summary}
The architecture of **WakaamaPlus (Wpp)** is a well-structured framework that provides a robust platform for the development and implementation of LwM2M client applications. It maintains a balance between high-level functionality and control, and is designed to be flexible and adaptable to various user and environmental needs.



