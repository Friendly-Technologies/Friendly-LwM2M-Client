\mainpage

**Welcome to the WakaamaPlus documentation.** Here you will find an overview of the project, build instructions, architecture details, usage examples, and code documentation.

- [Build Instructions](@ref build_tag)
- [Architecture](@ref architecture_tag)
- [Examples of Usage](@ref examples_tag)
- [Testing](@ref testing_tag)
- [Code Documentation](@ref code_tag)

### Introduction to WakaamaPlus
**WakaamaPlus (Wpp)** is an advanced extension of the open-source [**Wakaama**](https://github.com/eclipse/wakaama) project. **Wpp**  was created as a superstructure over [**Wakaama**](https://github.com/eclipse/wakaama) and its main value lies in its registry with implemented objects and their business logic. **Wpp** does not alter the behavior of the LwM2M and [**Wakaama**](https://github.com/eclipse/wakaama) projects, instead providing a robust framework for developing LwM2M client applications.

**Wpp** was developed as a cross-platform library that can be distributed in the form of source files or a static library with flexible configuration of the objects registry thanks to the developed set of utilities. **Wpp** contains no external dependencies except Wakaama and **C++ STL**. The **C++17 (C++1z)** standard is used.