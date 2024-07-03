\mainpage

**Welcome to the Friendly LWM2M client documentation.** Here you will find an overview of the project, build instructions, architecture details, usage examples, and code documentation.

- [Build Instructions](@ref build_tag)
- [RPi Setup](@ref rpi_setup_tag)
- [Architecture](@ref architecture_tag)
- [Examples of Usage](@ref examples_tag)
- [Testing](@ref testing_tag)
- [Code Documentation](@ref code_tag)

### Introduction to Friendly LWM2M client
**Friendly LWM2M client (Wpp)** is an advanced extension of the open-source [**Wakaama**](https://github.com/eclipse/wakaama) project. **Wpp**  was created as a superstructure over [**Wakaama**](https://github.com/eclipse/wakaama) and its main value lies in its registry with implemented objects and their business logic. **Wpp** does not alter the behavior of the LwM2M and [**Wakaama**](https://github.com/eclipse/wakaama) projects, instead providing a robust framework for developing LwM2M client applications.

**Wpp** was developed as a cross-platform library that can be distributed in the form of source files or a static library with flexible configuration of the objects registry thanks to the developed set of utilities. **Wpp** contains no external dependencies except Wakaama and **C++ STL**. The **C++17 (C++1z)** standard is used.

### Client Specifications

**Licence**: EPL 2.0 and BSD-3\n
**LwM2M Version**: 1.1\n
**Supported  LwM2M OMA Objects**: LWM2M Security(v1.1), LwM2M Server(v1.1), Device(v1.2), LwM2M Access Control(v1.1), Connectivity Monitoring(v1.3), Firmware Update(v1.1)\n
**Transport**: UDP/DTLS\n
**Data Formats**: Plain Text, Opaque, CBOR, TLV, JSON, SenML JSON, SenML CBOR\n
**Security**: PSK, Raw Public Key, No-Sec mode\n
**Supported platforms**: Linux\n

**Supported Interfaces & Features**:
    - **Bootstrap**: Request, Finish, Discover, Read, Write, Delete
    - **Registration**: Register, Update, De-register
    - **Device Management & Service Enablement**: Read, Discover, Write, Write-Attributes, Execute, Create, Delete
    - **Information reporting**: Observe, Notify, Cancel Observation, Send Operation

**Additional fetaures**: 
    - Utilities for generating the implementation of objects according to the description from the LwM2M Registry.
    - Implemented Access control
    - Implemented Firmware update
    - Implemented Conectivity monitoring 
    - CoAP Block transfer

### Memory consumption
When analyzing the measurement results, it should be noted that the measurements were conducted on **WppExample**, which includes not only the **Wpp** library but also user code that can significantly impact the size. For example, the tinydtls library is not part of **Wpp**, but it is used by **WppExample** to implement DTLS, and its size is about 46kB. This size may vary depending on the architecture's bitness, the functionality used, and the implementation of user code.

<table>
<caption id="multi_row">Memory usage</caption>
<tr><th>Mode <th>Wpp logs <th>Wakaama logs <th>DTLS <th>Bootstrap <th>Data formats <th>Objects <th>RAM <th>Flash
<tr><td>Minimal <td>Off <td>Off <td>Off <td>Off <td>TLV <td>Security(0), Server(1), Device(3) <td>8.1 kB <td>91 kB
<tr><td>Average <td>ERR <td>Off <td>On <td>Off <td>TLV, SenML CBOR <td>Security(0), Server(1), Device(3),Connectivity(4), Access(2), Audio Clip(3339), Firmware (5) <td>21.2 kB <td>181.5 kB
<tr><td>Full <td>On <td>On <td>On <td>On <td>TLV, SenML CBOR, CBOR, SenML JSON, JSON <td>Security(0), Server(1), Device(3), Connectivity(4), Access(2), Audio Clip(3339), Firmware (5) <td>26 kB <td>255.7 kB
</table>