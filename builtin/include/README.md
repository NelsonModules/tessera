# tessera builtin headers

This directory holds the generated C++ builtin headers for the `tessera`
module. It is populated by `ngen.gateway(FUNCTIONS, '<path-to-tessera>')`,
which writes one `<name>Builtin.hpp` per exposed native function here.
