//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "lloydBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
lloydBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 4);
    nargoutcheck(nLhs, 0, 2);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int cells = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"cells") : 16;
    int iterations = (argIn.size() > 2) ? tesseraScalarInt(argIn[2], L"iterations") : 2;
    unsigned int seed
        = (argIn.size() > 3) ? static_cast<unsigned int>(tesseraScalarInt(argIn[3], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    ArrayOf sites = ArrayOf::doubleMatrix2d(cells, 2);
    tessera_lloyd_voronoi(rows, cols, cells, iterations, seed,
        static_cast<double*>(out.readWriteDataPointer()),
        nLhs > 1 ? static_cast<double*>(sites.readWriteDataPointer()) : nullptr);
    ArrayOfVector retval;
    retval << out;
    if (nLhs > 1) {
        retval << sites;
    }
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
