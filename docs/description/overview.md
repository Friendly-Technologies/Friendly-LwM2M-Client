\mainpage

<strong>Welcome to the WakaamaPlus documentation.</strong> Here you will find an overview of the project, build instructions, architecture details, usage examples, and code documentation.

- [Build Instructions](@ref build_tag)
- [Architecture](@ref architecture_tag)
- [Examples of Usage](@ref examples_tag)
- [Code documentation](@ref code_tag)

## Introduction to WakaamaPlus
<strong>WakaamaPlus (Wpp)</strong> is an advanced extension of the open-source [<strong>Wakaama</strong>](https://github.com/eclipse/wakaama) project. <strong> Wpp</strong>  was created as a superstructure over [<strong>Wakaama</strong>](https://github.com/eclipse/wakaama) and its main value lies in its registry with implemented objects and their business logic. <strong>Wpp</strong> does not alter the behavior of the LwM2M and [<strong>Wakaama</strong>](https://github.com/eclipse/wakaama) projects, instead providing a robust framework for developing LwM2M client applications.

<strong>Wpp</strong> was developed as a cross-platform library that can be distributed in the form of source files or a static library with flexible configuration of the objects registry thanks to the developed set of utilities. <strong>Wpp</strong> contains no external dependencies except Wakaama and <strong>C++ STL</strong>. The <strong>C++17 (C++1z)</strong> standard is used.