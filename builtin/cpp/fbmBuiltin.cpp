//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "fbmBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
fbmBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 7);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int octaves = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"octaves") : 5;
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    double frequency = (argIn.size() > 3) ? tesseraScalarDouble(argIn[3], L"frequency") : 4.0;
    double lacunarity = (argIn.size() > 4) ? tesseraScalarDouble(argIn[4], L"lacunarity") : 2.0;
    double gain = (argIn.size() > 5) ? tesseraScalarDouble(argIn[5], L"gain") : 0.5;
    int seamless = (argIn.size() > 6) ? tesseraScalarInt(argIn[6], L"seamless") : 0;
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_fbm(rows, cols, octaves, frequency, lacunarity, gain, seed,
        seamless, static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
