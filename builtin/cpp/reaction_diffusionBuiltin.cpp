//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "reaction_diffusionBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
reaction_diffusionBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 5);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int steps = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"steps") : 2000;
    double feed = (argIn.size() > 2) ? tesseraScalarDouble(argIn[2], L"feed") : 0.055;
    double kill = (argIn.size() > 3) ? tesseraScalarDouble(argIn[3], L"kill") : 0.062;
    unsigned int seed
        = (argIn.size() > 4) ? static_cast<unsigned int>(tesseraScalarInt(argIn[4], L"seed")) : 0u;
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_reaction_diffusion(
        rows, cols, steps, feed, kill, seed, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
