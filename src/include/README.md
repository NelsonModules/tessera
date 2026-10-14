# tessera native headers

This directory holds the public C/C++ headers for the native sources in
`src/cpp`. It is always added to the compiler include path used by `builder.m`,
so a header dropped here is visible to both the native sources and the generated
`builtin/cpp` wrappers.
