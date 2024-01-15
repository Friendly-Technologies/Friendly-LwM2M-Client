/** \defuser Group0 This is group 0.
@ingroup users
 @{

 */
/** \defgroup Group1 This is group 1.
@ingroup usecases
@{ */
/** \defgroup Group2 This is group 2.
@ingroup usecases
@{ */
/** \defgroup Group3 This is group 3.
@ingroup usecases
@{ */

int main();

/** @} */
/** @} */
/** @} */
/** @} */

With this, the modules index (page 5 if this is the only input file) looks
like
the following:

Chapter 1
Module Index
1.1 Modules
Here is a list of all modules:
This is group 0. . . . . . . . . . . . . . . . . . . 3
   This is group 1 . . . . . . . . . . . . . . . . . 4
      This is group 2. . . . . . . . . . . . . . . . 5
         This is group 3 . . . . . . . . . . . . . . 6

Perfect!  But the main index (page 3) appears as the following:

Contents
1 Module Index 1
   This is group 0. . . . . . . . . . . . . . . . . . 3
   This is group 1. . . . . . . . . . . . . . . . . . 4
   This is group 2. . . . . . . . . . . . . . . . . . 5
   This is group 3. . . . . . . . . . . . . . . . . . 6   
2 Module Documentation 3


\tableofcontents limited to the top level headings only, such that only the
main
sections are bought out and the contents appears as the following:

Contents
1 Module Index 1
2 Module Documentation 3

This also has the benefit of reducing the size of this document if the
indices
are very large.  I've done this with the LaTeX command
"\setcounter{tocdepth}{0}".

