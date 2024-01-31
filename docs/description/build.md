\page build_tag Build Instructions for WakaamaPlus


This document provides detailed instructions on how to build the WakaamaPlus project. It covers requirements, setup steps, and common issues.
- [Setup build environment](@ref b_setupbenv)
  - [Repos Structure](@ref b_repostruct)
  - [Environment Setup](@ref b_envsetup)
  - [Source code](@ref b_sourcecode)
  - [VS Code installation](@ref b_vscodeinstall)
***
- [Implementation and customisation](@ref b_implandcustmn)
  - [Definitions and configurations](@ref b_defandconf)
  - [Static WPP library](@ref b_staticvar)
  - [Source WPP library](@ref b_sourcevar)


# Setup build environment {#b_setupbenv}

***
## Repos Structure {#b_repostruct}

The current \[07.06.2023\] structure contains only 3 repos (other will be created on ad-hoc basis):

* _WakaamaPlus_ Main Development Repo: [https://github.com/sinai-io/2305-WakaamaPlus](https://github.com/sinai-io/2305-WakaamaPlus)
* _Wakaama Upstream_ repo [https://github.com/eclipse/wakaama](https://github.com/eclipse/wakaama)
* _Wakaama Copy_ \[as on 07.06.2023\]: [https://github.com/sinai-io/2305-Wakaama](https://github.com/sinai-io/2305-Wakaama)

\image html build_1.png width=800

***
## Environment Setup {#b_envsetup}


### Automatic setup
The simpliest way is to use automatic setup by provided script - see [Automatic Setup](./../../wpp_env_setup.sh).

However this script is only helper and not heavy tested, so in case of problems, below you will find the full description of Environment Setup (skip the Automatic Setup chapter).

***
## Source code {#b_sourcecode}


### WakaamaPlus source code

Clone repository with submodules:
```
git clone --recurse-submodules git@github.com:sinai-io/2305-WakaamaPlus.git
```


### Wakaama source code

Clone [repository](https://github.com/eclipse/wakaama) with submodules: 
```
git clone --recurse-submodules git@github.com:eclipse/wakaama.git
```

***
## VS Code installation {#b_vscodeinstall}


### Environment

Visual Studio Code: 1.80.1 / **1.81.1**

Ubuntu: 18.04.6 / **22.04**


### Project setup stages

* Download [Visual Studio Code](https://code.visualstudio.com/download).
* Download the repository (`git clone --recurse-submodules git@github.com:sinai-io/2305-WakaamaPlus.git`).
* Go to the **2305-WakaamaPlus** folder.
* Run the `wpp_env_setup.sh` script to install the necessary utilities.
* Start **VSCode** and go to **File → Open Workspace from File**.
* In the opened window, specify the path to the file `vs-code-wakaamaplus.code-workspace` located in the **2305-WakaamaPlus** repository.
* After the workspace has loaded, go to **Extensions** `(Ctrl+Shift+x)`, set **Recommended** `(Text field: @recommended)` in the extensions filter, and then install all recommended extensions.

\image html build_2.png

* Let’s move on to the **Cmake extension**, the open one contains the entire main interface of the CMake Tool extension.

\image html build_3.png

* Before the first build of the projects, it is necessary to configure the Kit in both projects, for this you need to choose **Wpp Linux Kit** (but in general the compiler depends on the target platform) as a compiler for both projects. After the kit is installed, you can start the first building.

If you use Ubuntu with the 18 version and have trouble running `waakama_env.sh` - use the next commands and try again to run `waakama_env.sh`:
```
sudo apt-get install libstdc++-13-dev
sudo apt install clang-14
```

If you can not install **clang** or **clang-format** run the next commands:
```
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 14
sudo apt install clang-format-14
```


### Notes

\image html build_4.png

**CMake Tool:** 1 - allows you to select oneof the projects in the workspace, 2 - allows you to select the tool kit (the compiler with which the project will be compiled) and the build variant (Release, Debug), 3 - allows you to set the target (all, or some specific variant), 4 - runs the existing tests, 5 - starts the debug for the selected file, 6 - starts the execution of the selected file.

If you need to add custom Kit or modify the list of available kits it can be done in file vs- code-cmake-kits.json, that describes available kits. Also when you select needed kit in VS code, in addition to Wpp Linux Kit you can see other available on the PC that can be selected, if you need you can use them.

\image html build_5.png

### Problems&Solutions

#### Cmake Tool PROJECT STATUS is not displayed
> **Solution:** reload the CMake Tool extension.

#### Cmake Tools doesn't want to configure projects (no button for it)
> **Solution:** reinstall CMake and Cmake Tools extensions.


# Implementation and customisation {#b_implandcustmn}

***
## Definitions and configurations {#b_defandconf}

We have 4 configuration types. And all of these configs (defines) developer can redefine/modify or implement to project by another method. Be free and careful.<br />
Important, if you change configs in wpp/configs - all changes are applied to another part of the repository.<br />
Best practices, create or copy configs and use your custom configurations.

- [Compiler configuration](@ref b_compilerconf)
- [Object configuration](@ref b_objectconf)
- [Wakaama configuration](@ref b_wakaamaconf)
- [WPP configuration](@ref b_wppconf)

\image html build_6.png width=1000

### Compiler configuration {#b_compilerconf}

PATH - `wpp/configs/compiler_config.cmake`

#### Build options

**WPP_BUILD_WITH_EXCEPTIONS** - enable support of Exceptions (default: **OFF**)<br />
**WPP_BUILD_WITH_RTTI** - enable support of RTTI (default: **OFF**)<br />
**WPP_BUILD_FOR_64_BIT** - build for 64-bit system or 32-bit (default: **ON**)<br />
**CMAKE_POSITION_INDEPENDENT_CODE** - whether to create a position-independent target (default: **ON**) 

#### Compiler options

**Waggregate-return** - warn if any functions that return structures or unions are defined or called.<br />
**Wall** - this enables all the warnings about constructions that some users consider questionable, and that are easy to avoid, even in conjunction with macros.<br />
**Wcast-align** - warn whenever a pointer is cast such that the required alignment of the target is increased.<br />
**Wextra** - this enables some extra warning flags that are not enabled by -Wall.<br />
**Wfloat-equal** - warn if floating-point values are used in equality comparisons.<br />
**Wpointer-arith** - warn about anything that depends on the “size of” a function type or of void.<br />
**Wshadow** - warn whenever a local variable or type declaration shadows another variable, parameter, type, class member (in C++), or instance variable (in Objective-C) or whenever a built-in function is shadowed.<br />
**Wswitch-default** - warn whenever a switch statement does not have a default case.<br />
**Wwrite-strings** - these warnings help you find at compile time code that can try 
to write into a string constant, but only if you have been very careful about using const in declarations and prototypes.<br />
**Wno-unused-parameter** - unused parameters are common in this ifdef-littered code-base, but of no danger.<br />
**Wno-uninitialized** - too many false positives.<br />
**Wno-gnu-zero-variadic-macro-arguments** - allow usage ##__VA_ARGS__ in macros.<br />
**pedantic** - issue all the warnings demanded by strict ISO C and ISO C++; diagnose all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++.<br />
**Werror** - turn (most) warnings into errors.<br />
**Wno-error=cast-align** - disabled because of existing, non-trivially fixable code.

### Object configuration {#b_objectconf}

PATH - `wpp/configs/objects_config.cmake`

#### Mandatory objects config

**OBJ_M_3_DEVICE_V12** - include mandatory Device object in the build (default: **ON**)<br />
**OBJ_M_1_LWM2M_SERVER_V11** - include mandatory Lwm2mServer object in the build  (default: **ON**)<br />
**OBJ_M_0_LWM2M_SECURITY_V11** - include mandatory Lwm2mSecurity object in the build  (default: **ON**)

#### Optional objects config

**OBJ_O_4_CONNECTIVITY_MONITORING_V13** - include optional ConnectivityMonitoring object in the build  (default: **ON**)<br />
**OBJ_O_2_LWM2M_ACCESS_CONTROL_V11** - include optional Lwm2mAccessControl object in the build  (default: **ON**)<br />
**OBJ_O_5_FIRMWARE_UPDATE_V11** - include optional FirmwareUpdate object in the build  (default: **ON**)

### Wakaama configuration {#b_wakaamaconf}

PATH - `wpp/configs/wakaama_config.cmake`

**LWM2M_BOOTSTRAP** - enable LWM2M Bootstrap support in a LWM2M Client (default: **OFF**)<br />
**LWM2M_SUPPORT_SENML_JSON** - enable SenML JSON payload support (default: **OFF**)<br />
**LWM2M_SUPPORT_JSON** - enable JSON payload support (default: **OFF**)<br />
**LWM2M_SUPPORT_TLV** - enable TLV payload support (default: **ON**)<br />
**LWM2M_OLD_CONTENT_FORMAT_SUPPORT** - support the deprecated content format values for TLV and JSON (default: **OFF**)<br />
**LWM2M_VERSION_1_0** - support only version 1.0 (default: **OFF**)<br />
**LWM2M_RAW_BLOCK1_REQUESTS** - ror low memory client devices where it is not possible to keep a large post or put request in memory to be parsed. Control over such operations is provided entirely to the user. At the moment, there are certain restrictions regarding the use of this mode, only two operations are supported BLOCK_EXECUTE without restrictions, and BLOCK_WRITE with the following restrictions: recording only one SINGLE resource, recording is possible in the following formats: TEXT, OPAQUE, TLV. (default: **OFF**)<br />
**LWM2M_WITH_LOGS** - enable logs for wakaama core (default: **OFF**)<br />
**LWM2M_COAP_DEFAULT_BLOCK_SIZE** - CoAP block size used by CoAP layer when performing block-wise transfers. Possible values: 16, 32, 64, 128, 256, 512 and 1024  (default: **1024**)

Define your own endian if the endian is different from the platform default.<br />
`set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_BIG_ENDIAN)` - big-endian format<br />
`set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_LITTLE_ENDIAN)` - little-endian format

### WPP configuration {#b_wppconf}

PATH - `wpp/configs/wpp_config.cmake`

**WPP_ENABLE_LOGS** - enable logs for WakaamaPlus (default: **ON**)<br />
**WPP_LOGS_LEVEL** - set logs detalization for `WPP_ENABLE_LOGS ON` (default: **0**)

***
## Static WPP library {#b_staticvar}

**Definition:** The static build option in LwM2M indicates the process of compiling and assembling protocol code that includes all necessary libraries and dependencies, creating a single executable file with closed dependencies.<br />
**Advantages:** Ease of installation and deployment, as all dependencies are included in one file. Can be useful in resource-constrained environments where executable autonomy is desired.<br />
**Disadvantages:** Larger executable size, and updating dependencies may require recompiling the entire project.

For implementig library into your project:
1. Build static lib (*see image below*). Run **VS code** -> open **CMake** plugin -> **Set Build Target** Wpp -> run **Build**. Get archive `WppStaticLib.zip`.
2. Unzip `WppStaticLib.zip` (for example in folder `WppStaticLib`).
3. Add lib into the code in `<your_project>/CmakeLists.txt`  for starting to use WPP lib:
```
include(${CMAKE_SOURCE_DIR}/WppStaticLib/wpp_definitions.cmake)
target_include_directories(<YOUR_TARGET> PUBLIC ${CMAKE_SOURCE_DIR}/WppStaticLib/headers/)
target_compile_definitions(<YOUR_TARGET> PUBLIC ${WPP_DEFINITIONS})
target_link_libraries(<YOUR_TARGET> ${CMAKE_SOURCE_DIR}/WppStaticLib/libWpp.a)
```

\image html build_7.png width=1100

> **Static library realisation as in repository**<br />
> In repository implementation, we used a flexible variant where we can to modify all config and rebuild our WPP lib before we start building our destination code. If you want to use it change the code in the example folder.

***
## Source WPP library {#b_sourcevar}

**Definition:** The source code option in LwM2M indicates the use of the protocol source code without compiling it into a single executable file. This means that different components can be compiled independently of each other and then combined in an application.<br />
**Advantages:** Flexibility in choosing and updating dependencies, the ability to optimize and compile only the necessary components.<br />
**Disadvantages:** Requires more steps to deploy, but provides more flexibility in selecting components to compile.

For implementig library into your project in `<your_project>/CMakeLists.txt` (or see example `tests/CMakeLists.txt`):

1. Set definitions:
```
set(WPP_DEFINITIONS "")
```

2. Include definitions that will be needed to build the project.<br /> Create or copy configs and use your custom configurations:
```
# Include file with Wakaama core config
include(../wpp/configs/wakaama_config.cmake)
# Include file with wpp object configs
include(../wpp/configs/objects_config.cmake)
```
All are defined in a default state in the folder `wpp/configs`.

3. We link wpp source to our code:
```
# Add file with wpp lib configs
include(${CMAKE_SOURCE_DIR}/wpp/wpp.cmake)
```

4. Add definitions to our project: 
```
# Set properties
target_compile_definitions(<YOUR_EXECUTABLE> PUBLIC ${WPP_DEFINITIONS})
# Link wpp sources to tests
target_link_wpp(<YOUR_EXECUTABLE>)
```
 