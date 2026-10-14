//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "juliaBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
#include <complex>
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
juliaBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 2, 4);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    double cre = 0.0, cim = 0.0;
    const ArrayOf& cArg = argIn[1];
    if (cArg.isComplex()) {
        std::complex<double> z = cArg.asDoubleComplexScalar();
        cre = z.real();
        cim = z.imag();
    } else {
        std::vector<double> v = cArg.asDoubleVector();
        if (v.size() == 2) {
            cre = v[0];
            cim = v[1];
        } else if (v.size() == 1) {
            cre = v[0];
            cim = 0.0;
        } else {
            Error(L"tessera:julia:invalidConstant", L"c must be a complex scalar or a [real imag] vector.");
        }
    }
    int maxIter = (argIn.size() > 2) ? tesseraScalarInt(argIn[2], L"maxIter") : 100;
    double xmin = -1.6, xmax = 1.6, ymin = -1.2, ymax = 1.2;
    if (argIn.size() > 3) {
        tesseraParseRegion(argIn[3], xmin, xmax, ymin, ymax);
    }
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_julia(rows, cols, cre, cim, xmin, xmax, ymin, ymax, maxIter,
        static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
