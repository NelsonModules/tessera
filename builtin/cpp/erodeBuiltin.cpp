//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "erodeBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
erodeBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 2, 3);
    nargoutcheck(nLhs, 0, 1);
    const ArrayOf& hArg = argIn[0];
    if (!hArg.isNumeric() || hArg.isEmpty()) {
        Error(L"tessera:erode:invalidHeightfield", L"heightfield must be a non-empty numeric matrix.");
    }
    int rows = static_cast<int>(hArg.rows());
    int cols = static_cast<int>(hArg.columns());
    if (rows < 2 || cols < 2) {
        Error(L"tessera:erode:tooSmall", L"heightfield must be at least 2x2.");
    }
    std::vector<double> input = hArg.asDoubleVector();
    int droplets = tesseraScalarInt(argIn[1], L"droplets");
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_erode(
        input.data(), rows, cols, droplets, seed, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
