\page architecture_tag

## Architecture Description

-----------

- [Overview](@ref arch_overview)
- [Components](@ref arch_components)
- [User Interfaces](@ref arch_user_interfaces)

### Overview {#arch_overview}
**WakaamaPlus**, also known as **Wpp**, is a project built upon the [**Wakaama**](https://github.com/eclipse/wakaama) project, which implements the **Lightweight Machine to Machine (LwM2M)** protocol. The architecture of **Wpp** is carefully designed to provide a high-level presentation of [**Wakaama**](https://github.com/eclipse/wakaama) functionalities while retaining control and flexibility.

### Components {#arch_components}
Since **Wpp** is built upon the [**Wakaama**](https://github.com/eclipse/wakaama), the existing interfaces and capabilities of [**Wakaama**](https://github.com/eclipse/wakaama) affect the internal structure of **Wpp**. For a better understanding of what components should be present in **Wpp**, the [**Wakaama**](https://github.com/eclipse/wakaama) interface was segregated, based on it, the necessary components for **Wpp** were created.

\image html images/wakaama_core_api_segregation.png width=950px

**Wpp's** architecture is composed of several key components, each serving a distinct role:

1. **State Management (wpp::WppClient)**: This component is responsible for the initialization of the [**Wakaama**](https://github.com/eclipse/wakaama) core, processing protocol states, managing server and object registrations.
   
2. **Object Model (wpp::Object wpp::Instance wpp::Resource)**: This component is at the heart of **Wpp's** architecture. It defines the representation of objects at the **Wpp** level, their structure, general functionality, and sets the interface for interaction with instances and resources of the object, updating [**Wakaama**](https://github.com/eclipse/wakaama) about resource changes made by the user.

3. **Object Registry (wpp::WppRegistry)**: A crucial component that functions as a registry for available LwM2M objects. It manages the creation and initialization of objects and allows the user to create Instances and manage their resources.

4. **Communication (wpp::WppConnection)**: It defines the interface for establishing connections with the server and handling data transmission.

5. **Platform (wpp::WppPlatform wpp::WppGuard)**: This defines the interface for environment-dependent functions like memory allocation, logs printing, and time retrieval, and is entirely implemented by the user.

6. **Utils (folder utils)**: Includes various tools that is used by **Wpp**.

The diagram below shows the common **Wpp** components with their dependencies on [**Wakaama**](https://github.com/eclipse/wakaama). A part of the [**Wakaama**](https://github.com/eclipse/wakaama) library with its components is highlighted in **yellow**, and a part of the **Wpp** library is highlighted in **green**. The dependencies of the environment and the user program from the **Wpp** library are also indicated.

\image html images/wpp_dependencies_from_wakaama.jpg width=650px

### User Interfaces {#arch_user_interfaces}
Wpp includes three main interfaces:

1. **WppConnection**: Implemented by the user, it enables the opening of a connection with the server and facilitates data transfer.

2. **WppPlatform**: Also implemented by the user, it allows Wpp to receive the current system time and send logs.

3. **WppClient**: Processes the internal state of the object registry and provides access to WppRegistry, which contains implemented objects.

#### LwM2M Protocol and Proxy Objects
Wpp adheres to the principles of the LwM2M protocol, where interaction occurs between proxy objects on the server side and their real representations on the client side. Proxy objects do not implement the internal logic of client system components but act as their representatives, adhering to the standards defined for LwM2M objects.

#### Integration and Flexibility
Wpp's architecture allows for easy integration into new environments and the possibility to redefine interfaces as per environmental requirements. This flexibility is attributed to the absence of external dependencies, as Wpp defines all input and output interfaces, and the environment implements them.

### Summary
The architecture of WakaamaPlus (Wpp) is a well-structured framework that provides a robust platform for the development and implementation of LwM2M client applications. It maintains a balance between high-level functionality and control, and is designed to be flexible and adaptable to various user and environmental needs.



