//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "haltonBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
haltonBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 2);
    nargoutcheck(nLhs, 0, 1);
    int n = tesseraScalarInt(argIn[0], L"n");
    if (n < 1) {
        Error(L"tessera:count:invalidN", L"n must be a positive integer.");
    }
    int dims = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"dims") : 2;
    if (dims < 1 || dims > 16) {
        Error(L"tessera:halton:invalidDims", L"dims must be an integer in the range [1, 16].");
    }
    ArrayOf out = ArrayOf::doubleMatrix2d(n, dims);
    tessera_halton(n, dims, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
