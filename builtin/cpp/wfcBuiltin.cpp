//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "wfcBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
wfcBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 2, 4);
    nargoutcheck(nLhs, 0, 3);
    const ArrayOf& sArg = argIn[0];
    if (!sArg.isNumeric() || sArg.isEmpty()) {
        Error(L"tessera:wfc:invalidSample", L"sample must be a non-empty numeric matrix.");
    }
    int srows = static_cast<int>(sArg.rows());
    int scols = static_cast<int>(sArg.columns());
    std::vector<double> sample = sArg.asDoubleVector();
    int outRows = 0, outCols = 0;
    tesseraParseSize(argIn[1], outRows, outCols);
    int N = (argIn.size() > 2) ? tesseraScalarInt(argIn[2], L"N") : 2;
    if (N < 1 || srows < N || scols < N) {
        Error(L"tessera:wfc:sampleTooSmall", L"sample must be at least N x N.");
    }
    unsigned int seed
        = (argIn.size() > 3) ? static_cast<unsigned int>(tesseraScalarInt(argIn[3], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(outRows, outCols);
    int retries = 0;
    int contradictions = 0;
    double maxEntropy = 0.0;
    int traceCount = 0;
    ArrayOf trace = ArrayOf::doubleMatrix2d(outRows * outCols, 3);
    int success = tessera_wfc(sample.data(), srows, scols, N, outRows, outCols, seed,
        static_cast<double*>(out.readWriteDataPointer()), &retries, &contradictions, &maxEntropy,
        static_cast<double*>(trace.readWriteDataPointer()), outRows * outCols, &traceCount);
    ArrayOfVector retval;
    retval << out;
    if (nLhs > 1) {
        stringVector names { "success", "retries", "contradictions", "maxEntropy", "traceCount" };
        ArrayOfVector values;
        values << ArrayOf::doubleScalar(static_cast<double>(success));
        values << ArrayOf::doubleScalar(static_cast<double>(retries));
        values << ArrayOf::doubleScalar(static_cast<double>(contradictions));
        values << ArrayOf::doubleScalar(maxEntropy);
        values << ArrayOf::doubleScalar(static_cast<double>(traceCount));
        retval << ArrayOf::structScalar(names, values);
    }
    if (nLhs > 2) {
        retval << trace;
    }
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
