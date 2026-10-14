//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "diamond_squareBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
diamond_squareBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 3);
    nargoutcheck(nLhs, 0, 1);
    int n = tesseraScalarInt(argIn[0], L"n");
    if (n < 1) {
        Error(L"tessera:diamond_square:invalidN", L"n must be a positive integer (the field side is (2^n)+1).");
    }
    double roughness = (argIn.size() > 1) ? tesseraScalarDouble(argIn[1], L"roughness") : 0.5;
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    int side = (1 << n) + 1;
    ArrayOf out = ArrayOf::doubleMatrix2d(side, side);
    tessera_diamond_square(n, roughness, seed, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
