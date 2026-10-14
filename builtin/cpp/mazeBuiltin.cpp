//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "mazeBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
mazeBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 2, 3);
    nargoutcheck(nLhs, 0, 1);
    int rows = tesseraScalarInt(argIn[0], L"rows");
    int cols = tesseraScalarInt(argIn[1], L"cols");
    if (rows < 1 || cols < 1) {
        Error(L"tessera:maze:invalidSize", L"rows and cols must be positive integers.");
    }
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(2 * rows + 1, 2 * cols + 1);
    tessera_maze(rows, cols, seed, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
