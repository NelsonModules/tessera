//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "caveBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
caveBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 4);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    double fill = (argIn.size() > 1) ? tesseraScalarDouble(argIn[1], L"fill") : 0.45;
    int steps = (argIn.size() > 2) ? tesseraScalarInt(argIn[2], L"steps") : 5;
    unsigned int seed
        = (argIn.size() > 3) ? static_cast<unsigned int>(tesseraScalarInt(argIn[3], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_cave(rows, cols, fill, steps, seed, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
