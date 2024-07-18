# Friendly LwM2M Client [<img align="right" height="100px" src="https://github.com/Friendly-Technologies/Friendly-LwM2M-Client/blob/main/logo.png">](https://hubs.li/Q02D_GnB0)
This distribution contains **Friendly LwM2M client** as a reference for how to use Lightweight machine-to-machine implementations for IoT devices. 
This code is provided under the associated 
[LICENSE](LICENSE)

## Table of Contents

- [Friendly LwM2M Client ](#friendly-lwm2m-client-)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Documentation](#documentation)
  - [Features](#features)
    - [Supported features](#supported-features)
  - [Getting Started with Ubuntu](#getting-started-with-ubuntu)
    - [Prerequisites](#prerequisites)
    - [Setup build environment](#setup-build-environment)
    - [Command line](#command-line)
    - [VS Code](#vs-code)
    - [Configuration](#configuration)
  - [Getting Started with RPi 4](#getting-started-with-rpi-4)
    - [Prerequisites](#prerequisites-1)
    - [Setup build environment](#setup-build-environment-1)
    - [Command line](#command-line-1)
    - [VS Code](#vs-code-1)
    - [Configuration](#configuration-1)
  - [Contributing](#contributing)
  - [License](#license)

## Introduction

Friendly LwM2M Client is an open-source Lightweight machine-to-machine (LwM2M) client implementation designed for IoT devices. LwM2M is a protocol specified by the Open Mobile Alliance (OMA) for remote device management and service enablement for M2M and IoT devices and systems, intended for both resource-constrained and high-performance edge devices.

## Documentation 
[Friendly LWM2M Client](https://friendly-technologies.github.io/Friendly-LwM2M-Client/)
- [Build Instructions](https://friendly-technologies.github.io/Friendly-LwM2M-Client/build_tag.html)
- [RPi Setup](https://friendly-technologies.github.io/Friendly-LwM2M-Client/rpi_setup_tag.html)
- [Architecture](https://friendly-technologies.github.io/Friendly-LwM2M-Client/architecture_tag.html)
- [Examples of Usage](https://friendly-technologies.github.io/Friendly-LwM2M-Client/examples_tag.html)
- [Testing](https://friendly-technologies.github.io/Friendly-LwM2M-Client/testing_tag.html)
- [Code Documentation](https://friendly-technologies.github.io/Friendly-LwM2M-Client/code_tag.html)

## Features

- Lightweight and efficient implementation
- Supports LwM2M version 1.0 and 1.1
- Flexible configuration options
- Secure communication using DTLS
- Resource management and reporting
- Cross-platform support

### Supported features 

Supported  LwM2M OMA Objects

Mandatory 		
- LWM2M Security
- LwM2M Server
- Device

Optional 		
- LwM2M Access Control
- Connectivity Monitoring
- Firmware Update
- Supported IPSO Objects
		
Transport 		
- UDP/DTLS
		
Supported Interfaces & Features		

Bootstrap 		
- Request
- Finish
- Discover
- Read
- Write
- Delete

Registration 		
- Register
- Update
- De-register

Device Management & Service Enablement		
- Read
- Discover
- Write
- Execute
- Create
- Delete
- Send
		
Information reporting 		
- Observe
- Notify
- Cancel Observation
- Send Operation
		
Data Formats 		
- Plain Text
- Opaque
- CBOR
- TLV
- JSON
- SenML JSON
- SenML CBOR
		
Security 		
- PSK
- Raw Public Key
- No-Sec mode
		
Additional features 		
- Access control
- Connectivity monitoring
- Firmware update
- CoAP Block transfer


Note that additional features such as transport layers (TCP/TLS, SMS binding, MQTT, and HTTP transport binding); Composite operations, and Security modes (Certificate and Certificate mode with EST) are commercially available.

For more information please contact [Friendly Technologies](https://hubs.li/Q02D_GnZ0)

## Getting Started with Ubuntu

This section provides instructions on how to build and run the Friendly LwM2M client on the Ubuntu 22.04. Also, this section will describe the environment requirements, and the setup process for the build, and explain the two methods for building and running the Friendly LwM2M client.
The Friendly LwM2M client can be built and run in two different ways: using VS Code and using the command line.
The method using VS Code is suitable for developing and modifying example parameters, while the command line is suitable for quick building and exploring the capabilities.

Before proceeding with the description, it should also be noted that this guide will cover running an example that demonstrates the Friendly LwM2M client functionality. The client itself is provided as a library that can be built separately.

### Prerequisites

- OS Ubuntu 22.04
- [CMake](https://cmake.org/) version 3.10 or higher
- [OpenSSL](https://www.openssl.org/) for DTLS support
- C/C++ compiler (clang-14, clang++-14)

### Setup build environment

1. Clone the repository:
    ```sh
    git clone --recurse-submodules git@github.com:Friendly-Technologies/Friendly-LwM2M-Client.git
    cd Friendly-LwM2M-Client
    ```

2. Setup environment:
    ```sh
    sh ./wpp_env_setup.sh
    ```

During step **2**, documentation will also be automatically generated and launched in the browser, so for more information on the structure and usage of the Friendly LwM2M client, you can refer to it.

After successfully completing these steps, we will have a fully configured environment with the loaded source files. The next steps are to build the example and run it. As mentioned earlier, this can be done using one of the two methods: via VS Code or the command line. Each method will be detailed further.

### Command line

1. Create build directory:
    ```sh
    mkdir _build
    cd _build
    ```

2. Generate build scripts with CMake:
    ```sh
    cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
    ```

3. Building:
    ```sh
    cmake --build . --config MinSizeRel --target WppLinuxExample -j 14 --
    ```

4. Running:
    ```sh
    cd examples/linux
    ./WppLinuxExample
    ```

### VS Code

1. Download [Visual Studio Code](https://code.visualstudio.com/download).
2. Start VS Code and open workspace which is set up for this project `File → Open Workspace` from File.
3. In the open window, specify the path to the file `vs-code-wpp.code-workspace` located in the `Friendly-LwM2M-Client` repository.
4. After the workspace has loaded, go to `Extensions` (Ctrl+Shift+x), set `Recommended` (Text field: @recommended) in the extensions filter, and then install all recommended extensions.
5. Let’s move on to the `CMake Tool` extension, the open one contains the entire main interface.
6. Setup `Kit` for `Friendly-LwM2M-Client` project, to do this in the `Configure` section select `Wpp Linux Kit`.
7. Setup `Build Target`, to do this in the `Build` section select `WppLinuxExample`.
8. Press `Build` button
9. Launch the client, to do this in the `Launch` section select `WppLinuxExample`, and press `Run in Terminal` button.

### Configuration

The generated client example is configured using the source file: `Friendly-LwM2M-Client/examples/linux/objects.cpp`. After changing the configuration, the client example should be built again.

By default, `WppLinuxExample` has the following configurations:
1. COAP server: coaps://qa62.friendly-tech.com:5684.
2. DTLS enabled.
3. Data formats: CBOR, SENML CBOR, SENML JSON, JSON, TLV.
4. Objects: DEVICE, LWM2M SERVER, LWM2M SECURITY, CONNECTIVITY MONITORING, LWM2M ACCESS CONTROL, FIRMWARE UPDATE.

## Getting Started with RPi 4

This section provides instructions on how to build and run the Friendly LwM2M client on the RPi 4 with installed Ubuntu Server. Also, this section will describe the environment requirements, and the setup process for the build, and explain the two methods for building and running the Friendly LwM2M client.
The Friendly LwM2M client can be built and run in two different ways: using VS Code and using the command line.
The method using VS Code is suitable for developing and modifying example parameters, while the command line is suitable for quick building and exploring the capabilities.

### Prerequisites

Running environment:
- Raspberry Pi 4 Model B
- RPi Ubuntu Server 22.04

Building environment:
- OS Ubuntu 22.04
- [CMake](https://cmake.org/) version 3.10 or higher
- [OpenSSL](https://www.openssl.org/) for DTLS support
- C/C++ compiler (clang-14, clang++-14)

### Setup build environment

1. Clone the repository:
    ```sh
    git clone --recurse-submodules --branch RPi-4-demo git@github.com:Friendly-Technologies/Friendly-LwM2M-Client.git
    cd Friendly-LwM2M-Client
    ```

2. Setup environment:
    ```sh
    sh ./wpp_env_setup.sh
    ```

During step **2**, documentation will also be automatically generated and launched in the browser, so for more information on the structure and usage of the Friendly LwM2M client, you can refer to it.

After successfully completing these steps, we will have a fully configured environment with the loaded source files. The next steps are to build the example and run it. As mentioned earlier, this can be done using one of the two methods: via VS Code or the command line. Each method will be detailed further.

### Command line

1. Create build directory:
    ```sh
    mkdir _build
    cd _build
    ```

2. Generate build scripts with CMake:
    ```sh
    cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
    ```

3. Building:
    ```sh
    cmake --build . --config MinSizeRel --target WppRPiExample -j 14 --
    ```

4. Running:
    ```sh
    cd examples/raspberry_pi
    scp WppRPiExample wpp@<rpi_ip>:~/
    ssh wpp@<rpi_ip>
    sudo apt-get install libgpiod2
    sudo chmod +x WppRPiExample
    ./WppRPiExample
    ```

A detailed guide on flashing, configuring the RPi, creating a service for the automatic startup of WppRPiExample, and assembling the circuit can be found at the following link: [RPi Setup](https://friendly-technologies.github.io/Friendly-LwM2M-Client/rpi_setup_tag.html).

### VS Code

1. Download [Visual Studio Code](https://code.visualstudio.com/download).
2. Start VS Code and open workspace which is set up for this project `File → Open Workspace` from File.
3. In the open window, specify the path to the file `vs-code-wpp.code-workspace` located in the `Friendly-LwM2M-Client` repository.
4. After the workspace has loaded, go to `Extensions` (Ctrl+Shift+x), set `Recommended` (Text field: @recommended) in the extensions filter, and then install all recommended extensions.
5. Let’s move on to the `CMake Tool` extension, the open one contains the entire main interface.
6. Setup `Kit` for `Friendly-LwM2M-Client` project, to do this in the `Configure` section select `Wpp Linux Kit`.
7. Setup `Build Target`, to do this in the `Build` section select `WppRPiExample`.
8. Press `Build` button
9. Launch the client:
     ```sh
    cd _build/examples/raspberry_pi
    scp WppRPiExample wpp@<rpi_ip>:~/
    ssh wpp@<rpi_ip>
    sudo apt-get install libgpiod2
    sudo chmod +x WppRPiExample
    ./WppRPiExample
    ```

### Configuration

The generated client example is configured using the source file: `Friendly-LwM2M-Client/examples/raspberry_pi/objects.cpp`. After changing the configuration, the client example should be built again.

By default, `WppRPiExample` has the following configurations:
1. COAP server: coaps://qa62.friendly-tech.com:5684.
2. DTLS enabled.
3. Data formats: CBOR, SENML CBOR, SENML JSON, JSON, TLV.
4. Objects: DEVICE, LWM2M SERVER, LWM2M SECURITY, CONNECTIVITY MONITORING, LWM2M ACCESS CONTROL, FIRMWARE UPDATE.

## Contributing

We welcome contributions from the community! Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -m 'Add new feature'`).
5. Push to the branch (`git push origin feature-branch`).
6. Open a pull request.

Please ensure your code adheres to our [coding standards](CODE_OF_CONDUCT.md).

## License

This project is licensed under the following license- see the [LICENSE](LICENSE) file for details.
