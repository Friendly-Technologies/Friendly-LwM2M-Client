\page testing_tag Testing

For checking and testing we used several utils.<br />

Tool **Cppcheck** for static analysis for C/C++ code (version 2.7). It provides unique code analysis to detect bugs and focuses on detecting undefined behaviour and dangerous coding constructs.<br />
**Catch2** framework (version 3.4.0) for unit testing.<br />
**LLVM** tools (version 14.0.0) for shows code coverage. llvm-profdata merge for merging into the single indexed profile data file. And llvm-cov tool what the shows code coverage information for programs that are instrumented to emit profile data.<br />

We can run it in **VS Code** if **Set Build Target** is **All** or **Catch2Tests** and click **Build**. 

\image html test_1.png width=1400

After the build project process, the static analysis is automatically run, then the unit tests, and finally shows the overall coverage.

\image html test_2.png width=1400

For a detailed coverage report, open `index.html` in your browser (full path `_build/tests/coverage/index.html`).

\image html test_3.png width=1100
