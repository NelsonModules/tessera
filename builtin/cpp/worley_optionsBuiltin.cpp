//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "worley_optionsBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
worley_optionsBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 6);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int cells = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"cells") : 8;
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    int feature = (argIn.size() > 3) ? tesseraScalarInt(argIn[3], L"feature") : 1;
    int metric = (argIn.size() > 4) ? tesseraScalarInt(argIn[4], L"metric") : 0;
    int seamless = (argIn.size() > 5) ? tesseraScalarInt(argIn[5], L"seamless") : 0;
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_worley_options(
        rows, cols, cells, seed, feature, metric, seamless, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
