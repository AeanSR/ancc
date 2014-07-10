ancc
====

ancc means "Another Noobish C Compiler". This is a homework project, a project for the compiler principles course.
Some useful tools are included as a part of ancc project: anccc ("Another Noobish C Compiler's Compiler") is a Canonical LR(1) parser table generator. anlibc ("the ANcc C LIBrary") is a minimalist C Standard Library for win32 platform, without any #pragma or built-in.
ancc is designed to rely on MSVC's components, for preprocessor, assembler, and linker. I'd be happy if I have the time and ability rolling to my own. It's still a huge project even without these steps.

Mail to v(at)aean.net to contact me.

Building steps
====

1. Build anccc first
2. Run anccc to generate parsetbl.c
3. Copy parsetbl.c to the trunk folder
4. Build ancc, anlibc, etc.
