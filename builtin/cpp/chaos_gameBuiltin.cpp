//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "chaos_gameBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
chaos_gameBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 3);
    nargoutcheck(nLhs, 0, 1);
    int n = tesseraScalarInt(argIn[0], L"n");
    if (n < 1) {
        Error(L"tessera:count:invalidN", L"n must be a positive integer.");
    }
    int kind = 0;
    if (argIn.size() > 1) {
        std::wstring k = argIn[1].asWideString();
        if (k == L"fern") {
            kind = 0;
        } else if (k == L"sierpinski") {
            kind = 1;
        } else {
            Error(L"tessera:chaos_game:invalidKind", L"kind must be 'fern' or 'sierpinski'.");
        }
    }
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(n, 2);
    std::vector<double> xs(static_cast<size_t>(n));
    std::vector<double> ys(static_cast<size_t>(n));
    tessera_chaos_game(kind, n, seed, xs.data(), ys.data());
    double* data = static_cast<double*>(out.readWriteDataPointer());
    for (int i = 0; i < n; ++i) {
        data[i] = xs[i];
        data[i + n] = ys[i];
    }
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
