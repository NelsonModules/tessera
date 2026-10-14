//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "domain_warpBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
domain_warpBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 4);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int octaves = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"octaves") : 5;
    double strength = (argIn.size() > 2) ? tesseraScalarDouble(argIn[2], L"strength") : 4.0;
    unsigned int seed
        = (argIn.size() > 3) ? static_cast<unsigned int>(tesseraScalarInt(argIn[3], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_domain_warp(
        rows, cols, octaves, 4.0, strength, seed, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
