//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "automaton1dBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
automaton1dBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 3, 4);
    nargoutcheck(nLhs, 0, 1);
    int rule = tesseraScalarInt(argIn[0], L"rule");
    if (rule < 0 || rule > 255) {
        Error(L"tessera:automaton1d:invalidRule", L"rule must be an integer in the range [0, 255].");
    }
    int width = tesseraScalarInt(argIn[1], L"width");
    int generations = tesseraScalarInt(argIn[2], L"generations");
    if (width < 1 || generations < 1) {
        Error(L"tessera:automaton1d:invalidSize", L"width and generations must be positive integers.");
    }
    std::vector<double> init;
    int hasInit = 0;
    if (argIn.size() > 3 && !argIn[3].isEmpty()) {
        init = argIn[3].asDoubleVector();
        if (static_cast<int>(init.size()) != width) {
            Error(L"tessera:automaton1d:invalidInit", L"init must be a vector of length width.");
        }
        hasInit = 1;
    }
    unsigned int seed = 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(generations, width);
    tessera_automaton1d(rule, width, generations, hasInit ? init.data() : nullptr, hasInit, seed,
        static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
