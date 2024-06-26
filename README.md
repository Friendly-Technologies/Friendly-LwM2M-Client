# Friendly LwM2M Client [<img align="right" height="100px" src="https://github.com/Friendly-Technologies/Friendly-LwM2M-Client/blob/main/logo.png">](http://www.friendly-tech.com/)
This distribution contains Friendly LwM2M client as a reference for how to use Lightweight Machine to Machine implementations for IoT devices. 
This code is provided under the associated license: 
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Table of Contents

- [Introduction](#introduction)
- [Documentation](#documentation)
- [Features](#features)
- [Getting Started](#getting-started)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Friendly LwM2M Client is an open-source Lightweight machine-to-machine (LwM2M) client implementation designed for IoT devices. LwM2M is a protocol specified by the Open Mobile Alliance (OMA) for remote device management and service enablement for M2M and IoT devices and systems, intended for both resource-constrained and high-performance edge devices.

## Documentation 
[Friendly LWM2M Client Full Documentation](https://friendly-technologies.github.io/Friendly-LwM2M-Client/) 


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
- LWM2M Security	0	v
- LwM2M Server	1	v
- Device	3	v
Optional 		
LwM2M Access Control	2	v
Connectivity Monitoring	4	v
Firmware Update	5	v
		
Supported IPSO Objects 		v
		
Transport 		
UDP / DTLS		v
		
Supported Interfaces & Features 		
Bootstrap 		
Request 		v
Finish 		v
Discover 		v
Read 		v
Write 		v
Delete 		v
Registration 		
Register 		v
Update 		v
De-register		v
Device Management & Service Enablement		
Read		v
Discover 		v
Write 		v
Execute		v
Create 		v
Delete 		v
Send		v
		
Information reporting 		
Observe		v
Notify		v
Cancel Observation		v
Send Operation 		v
		
Data Formats 		
Plain Text		v
Opaque		v
CBOR		v
TLV		v
JSON		v
SenML JSON 		v
SenML CBOR 		v
		
Security 		
PSK 		v
Raw Public Key 		v
No-Sec mode 		v
		
Additional fetaures 		
Access control		v
Conectivity monitoring 		v
Firmware update 		v
CoAP Block transfer 		v


Note that additional features such as: transport layers (TCP/TLS, SMS binding, MQTT, and HTTP transport binding); Composite operations, and Security modes (Certificate and Certificate mode with EST) are commercially available. 
For more information please contact Friendly Technologies


##Getting Started 
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
