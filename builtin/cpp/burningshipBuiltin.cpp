//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "burningshipBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
burningshipBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 3);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int maxIter = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"maxIter") : 100;
    double xmin = -2.5, xmax = 1.5, ymin = -2.0, ymax = 1.0;
    if (argIn.size() > 2) {
        tesseraParseRegion(argIn[2], xmin, xmax, ymin, ymax);
    }
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_burningship(rows, cols, xmin, xmax, ymin, ymax, maxIter,
        static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
