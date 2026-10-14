//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "gameoflifeBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
gameoflifeBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 2);
    nargoutcheck(nLhs, 0, 1);
    const ArrayOf& gridArg = argIn[0];
    if (!gridArg.isNumeric() && !gridArg.isLogical()) {
        Error(L"tessera:gameoflife:invalidGrid", L"grid must be a numeric or logical matrix.");
    }
    if (gridArg.isEmpty()) {
        Error(L"tessera:gameoflife:emptyGrid", L"grid must be a non-empty matrix.");
    }
    int rows = static_cast<int>(gridArg.rows());
    int cols = static_cast<int>(gridArg.columns());
    std::vector<double> input = gridArg.asDoubleVector();
    int generations = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"generations") : 1;
    if (generations < 0) {
        Error(L"tessera:gameoflife:invalidGenerations", L"generations must be a non-negative integer.");
    }
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_gameoflife(
        input.data(), rows, cols, generations, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
