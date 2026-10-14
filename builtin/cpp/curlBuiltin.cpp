//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "curlBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
curlBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 3);
    nargoutcheck(nLhs, 0, 2);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    double frequency = (argIn.size() > 1) ? tesseraScalarDouble(argIn[1], L"frequency") : 8.0;
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    ArrayOf u = ArrayOf::doubleMatrix2d(rows, cols);
    ArrayOf v = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_curl(rows, cols, frequency, seed, static_cast<double*>(u.readWriteDataPointer()),
        static_cast<double*>(v.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << u;
    if (nLhs > 1) {
        retval << v;
    }
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
