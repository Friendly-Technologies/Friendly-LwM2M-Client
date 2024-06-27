# Friendly LwM2M Client [<img align="right" height="100px" src="https://github.com/Friendly-Technologies/Friendly-LwM2M-Client/blob/main/logo.png">](http://www.friendly-tech.com/)
This distribution contains Friendly LwM2M client as a reference for how to use Lightweight Machine to Machine implementations for IoT devices. 
This code is provided under the associated 
[LICENSE](LICENSE)

## Table of Contents

- [Introduction](#introduction)
- [Documentation](#documentation)
- [Features](#features)
- [Development Quick Start](#Development-Quick-Start)
- [Getting Started](#Getting-Started)
	- [Prerequisites](#Prerequisites)
	- [Building from Source](#Building-from-Source)
	- [Running](#Running)
 	- [Command-Line Options](#Command-Line-Options)
  	- [Configuration](#Configuration)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Friendly LwM2M Client is an open-source Lightweight machine-to-machine (LwM2M) client implementation designed for IoT devices. LwM2M is a protocol specified by the Open Mobile Alliance (OMA) for remote device management and service enablement for M2M and IoT devices and systems, intended for both resource-constrained and high-performance edge devices.

## Documentation 
[Friendly LWM2M Client](https://friendly-technologies.github.io/Friendly-LwM2M-Client/)
- [Build Instructions](https://friendly-technologies.github.io/Friendly-LwM2M-Client/build_tag.html)
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
For more information please contact Friendly Technologies



## Development Quick Start
For download repository with submodules:  `git clone --recurse-submodules git@github.com:Friendly-Technologies/Friendly-LwM2M-Client.git`
For running auto setup environment:  `sh ./wpp_env_setup.sh`

For open projects with VS Code:
1) Start VS Code and go to `File→Open Workspace` from File.
2) In the open window, specify the path to the file `vs-code-wakaamaplus.code-workspace` located in the `2305-WakaamaPlus` repository.
3) After the workspace has loaded, go to `Extensions` (Ctrl+Shift+x), set `Recommended` (Text field: @recommended) in the extensions filter, and then install all recommended extensions.
4) Let’s move on to the `CMake Tool` extension, the open one contains the entire main interface.
5) Before the first build of the projects, it is necessary to configure the `Kit` in both projects, for this you need to choose `Wpp Linux Kit` (but in general the compiler depends on the target platform) as a compiler for both projects. After the  kit is installed, you can start the first building.



## Getting Started 
This section should provide instructions on how to build and run the Friendly LwM2M client.
•	Prerequisites: List any required tools, libraries, or dependencies.
•	Building: Explain how to build the client from source code.
•	Running: Provide instructions on how to run the client application, including any configuration options.

### Prerequisites

- [CMake](https://cmake.org/) version 3.10 or higher
- [OpenSSL](https://www.openssl.org/) for DTLS support
- A C compiler (GCC, Clang, etc.)


### Building from Source

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/lwm2m-client.git
    cd lwm2m-client
    ```

2. Create a build directory and navigate to it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the build environment:
    ```sh
    cmake ..
    ```

4. Compile the source code:
    ```sh
    make
    ```

5. Install the client:
    ```sh
    sudo make install
    ```

### Running 

After installation, you can start the LwM2M client using the following command:

```sh
lwm2m-client --config /path/to/config.json
```

#### Command-Line Options

- `--config <path>`: Path to the configuration file (default: `config.json`).
- `--log-level <level>`: Set the log level (e.g., `info`, `debug`, `warn`, `error`).

#### Configuration

The client is configured using a JSON file. Below is an example configuration file:

```json
{
  "server": {
    "uri": "coaps://lwm2m-server.com:5684",
    "lifetime": 300,
    "security": {
      "psk": "your_pre_shared_key"
    }
  },
  "device": {
    "manufacturer": "YourCompany",
    "model": "YourModel",
    "serial": "1234567890"
  }
}
```

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
