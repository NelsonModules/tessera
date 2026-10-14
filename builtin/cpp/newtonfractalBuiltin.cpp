//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "newtonfractalBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
newtonfractalBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 3);
    nargoutcheck(nLhs, 0, 2);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int maxIter = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"maxIter") : 50;
    double xmin = -2.0, xmax = 2.0, ymin = -2.0, ymax = 2.0;
    if (argIn.size() > 2) {
        tesseraParseRegion(argIn[2], xmin, xmax, ymin, ymax);
    }
    ArrayOf root = ArrayOf::doubleMatrix2d(rows, cols);
    ArrayOf iter = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_newton(rows, cols, xmin, xmax, ymin, ymax, maxIter,
        static_cast<double*>(root.readWriteDataPointer()),
        static_cast<double*>(iter.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << root;
    if (nLhs > 1) {
        retval << iter;
    }
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
